#include "ATA.h"
#include "io.h"
#include "text_mode.h"

#define ATA_DATA_PORT         0x01F0
#define ATA_ERROR_PORT        0x01F1
#define ATA_FEATURES_PORT     0x01F1
#define ATA_SECTOR_COUNT_PORT 0x01F2
#define ATA_LBA_LOW_PORT      0x01F3
#define ATA_LBA_MID_PORT      0x01F4
#define ATA_LBA_HIGH_PORT     0x01F5
#define ATA_DEVICE_PORT       0x01F6
#define ATA_STATUS_PORT       0x01F7
#define ATA_COMMAND_PORT      0x01F7

#define ATA_SR_BSY  0x80 // Busy
#define ATA_SR_DRDY 0x40 // Drive ready
#define ATA_SR_DF   0x20 // Device fault
#define ATA_SR_DSC  0x10 // Drive seek complete
#define ATA_SR_DRQ  0x08 // Data request ready
#define ATA_SR_CORR 0x04 // Corrected data
#define ATA_SR_IDX  0x02 // Index
#define ATA_SR_ERR  0x01 // Error

u8 busy_loop(u8 status) {
  while ((status & ATA_SR_BSY) != 0)
    status = inb(ATA_STATUS_PORT);
  return status;
}

void ata_error() {
  print("ATA ERROR\n");
}

/* device = 0 | 1 */
u8 identify(u8 device, struct ATA_DEVICE_INFORMATION *info_ptr) {
  // Select device, then clear unused registers (LBA, sector_count) and finally send IDENTIFY comand
  outb(ATA_DEVICE_PORT, 0b10100000 | (device << 4));
  outb(ATA_SECTOR_COUNT_PORT, 0);
  outb(ATA_LBA_LOW_PORT, 0);
  outb(ATA_LBA_MID_PORT, 0);
  outb(ATA_LBA_HIGH_PORT, 0);
  outb(ATA_COMMAND_PORT, 0xEC);

  u8 status = inb(ATA_STATUS_PORT);

  if (status == 0)
    return NO_DEVICE;

  status = busy_loop(status);

  u16 id = inb(ATA_LBA_MID_PORT) | (inb(ATA_LBA_HIGH_PORT) << 8);
  if (id == 0xEB14) return PATAPI_DEVICE;
	if (id == 0x9669) return SATAPI_DEVICE;
	if (id == 0xc33c) return SATA_DEVICE;
  if (id != 0x0000) return UNKNOW_DEVICE;

  while ((status & (ATA_SR_DRQ | ATA_SR_ERR)) == 0)
    status = inb(ATA_STATUS_PORT);

  if (status & 1)
    ata_error();

  if (info_ptr != NULL) {
    u16 *data_out_ptr = (u16*)info_ptr;
    for (u32 i = 0; i < 256; i++) {
      u16 data = inw(ATA_DATA_PORT);
      *(data_out_ptr++) = data;
    }
    print_hex_w(info_ptr->general_configuration_bit); // Check if bit 15 is 0 => ATA
  } else
    for (u32 i = 0; i < 128; i++)
      ind(ATA_DATA_PORT);

  return PATA_DEVICE;
}

// LBA = LBA | (device << 24)
void read_sectors(u32 LBA, u8 sector_count, u32 *dest) {
  if (dest == NULL) return;
  u8 status;

  outb(ATA_SECTOR_COUNT_PORT, sector_count);
  outb(ATA_LBA_LOW_PORT, LBA & 0xFF);
  outb(ATA_LBA_MID_PORT, (LBA >> 8) & 0xFF);
  outb(ATA_LBA_HIGH_PORT, (LBA >> 16) & 0xFF);
  outb(ATA_DEVICE_PORT, (LBA >> 24) & 0xFF);
  outb(ATA_COMMAND_PORT, 0x20);

  status = busy_loop(ATA_SR_BSY);

  while ((status & (ATA_SR_DRQ | ATA_SR_ERR)) == 0)
    status = inb(ATA_STATUS_PORT);

  if (status & 1)
    ata_error();

  while (sector_count) {
    for (u32 i = 0; i < 128; i++)
      *(dest++) = ind(ATA_DATA_PORT);
    do
      status = inb(ATA_STATUS_PORT);
    while ((status & ATA_SR_BSY) != 0);
    sector_count--;
  }
}

void write_sectors(u32 LBA, u8 sector_count, u32 *src) {
  u8 status;

  outb(ATA_SECTOR_COUNT_PORT, sector_count);
  outb(ATA_LBA_LOW_PORT, LBA & 0xFF);
  outb(ATA_LBA_MID_PORT, (LBA >> 8) & 0xFF);
  outb(ATA_LBA_HIGH_PORT, (LBA >> 16) & 0xFF);
  outb(ATA_DEVICE_PORT, (LBA >> 24) & 0xFF);
  outb(ATA_COMMAND_PORT, 0x30);

  status = busy_loop(ATA_SR_BSY);

  while ((status & (ATA_SR_DRQ | ATA_SR_ERR)) == 0)
    status = inb(ATA_STATUS_PORT);

  if (status & 1)
    ata_error();

  while (sector_count) {
    for (u32 i = 0; i < 128; i++)
      outd(ATA_DATA_PORT, *(src++));
    do
      status = inb(ATA_STATUS_PORT);
    while ((status & ATA_SR_BSY) != 0);
    sector_count--;
  }
}

void flush_cache(u8 device) {
  outb(ATA_DEVICE_PORT, 0b10100000 | (device << 4));
  outb(ATA_COMMAND_PORT, 0xE7);

  u8 status;
  do
    status = inb(ATA_STATUS_PORT);
  while (status & ATA_SR_BSY);

  if (status & ATA_SR_ERR)
      ata_error();
}