#include "text_mode.h"
#include "interrupts.h"
#include "PCI.h"

const char *msg = "Hello from C kernel !\n";

void kernel_main() {
  clear_screen();

  print(msg);

  init_idt();

  map_PIC();
  set_PIC_mask(0x01, 0x00);

  test_pci();

}
