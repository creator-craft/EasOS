#include "PCI.h"
#include "types.h"
#include "io.h"
#include "debug.h"

#define PCI_CONFIG_ADDRESS_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

typedef union {
  struct {
    u16 high; // VENDOR_ID
    u16 low;  // DEVICE_ID
  };
  struct {
    u32 dword;
  };
} __attribute__((packed)) device_info;


device_info pci_read_config(u8 bus, u8 dev, u8 fun, u8 reg) {
  outd(PCI_CONFIG_ADDRESS_PORT, ((u32)bus << 16) | ((u32)dev << 11) | ((u32)fun << 8) | (0xFC & reg) | 0x80000000);
  device_info dev_info;
  __asm__ volatile ( "inl %1, %0" : "=a"(dev_info) : "Nd"(PCI_CONFIG_DATA_PORT) );
  return dev_info;
}

void test_pci() {
  for (u32 bus = 0; bus < 256; bus++)
    for (u8 dev = 0; dev < 32; dev++) {
      device_info dev_info = pci_read_config(bus, dev, 0, 0);
      if (dev_info.high != 0xFFFF) {
        debug_hex_w(dev_info.high);
        debug_char(':');
        debug_hex_w(dev_info.low);
        debug_char(' ');

        dev_info = pci_read_config(bus, dev, 0, 4);
        debug_hex_d(dev_info.dword);
        debug_char(' ');
        dev_info = pci_read_config(bus, dev, 0, 8);
        debug_hex_d(dev_info.dword);
        debug_new_line();

      }
    }
}