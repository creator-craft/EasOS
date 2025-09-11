#include "text_mode.h"
#include "interrupts.h"
#include "PCI.h"
#include "ATA.h"

const char *msg = "Hello from C kernel !\n";

struct ATA_DEVICE_INFORMATION *test = (void*)0x500;

void kernel_main() {
  clear_screen();

  print(msg);

  init_idt();

  map_PIC();
  // set_PIC_mask(0b11111001, 0b10111111);

  test_pci();

  // outb(0x3F6, 0x00); // Activate HDC ?

  print_hex_b(identify(0, NULL));

}
