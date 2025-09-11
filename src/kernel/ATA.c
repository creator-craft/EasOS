#include "ATA.h"
#include "io.h"
#include "text_mode.h"

#define ATA_DATA_PORT         0x01F0
#define ATA_ERROR_PORT        0x01F1
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
  outb(ATA_LBA_LOW_PORT, 0);
  outb(ATA_LBA_MID_PORT, 0);
  outb(ATA_LBA_HIGH_PORT, 0);
  outb(ATA_SECTOR_COUNT_PORT, 0);
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
    for (u32 i = 0; i < 255; i++) {
      u16 data = inw(ATA_DATA_PORT);
      *(data_out_ptr++) = data;
    }
    print_hex_w(info_ptr->general_configuration_bit); // Check if bit 15 is 0 => ATA
  }

  return PATA_DEVICE;
}

