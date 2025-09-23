#include "text_mode.h"
#include "interrupts.h"
#include "PCI.h"
#include "ATA.h"
#include "PIT.h"
#include "mouse.h"
#include "tests.h"

const char *msg = "Hello from C kernel !\n";

void kernel_main() {
  clear_screen();

  print(msg);

  init_idt();

  map_PIC();
  set_PIC_mask(0xFF, 0xFF);

  print("Mouse init: ");
  print_hex_b(init_mouse(10, 1));
  print_new_line();

  set_PIC_mask(PIC_CASCADE & PIC_KEYBOARD & PIC_PIT, PIC_ATA1 & PIC_MOUSE);

  test_sleep();

  test_procs();

  test_pci();

  print_hex_b(identify(0, NULL));
  print_new_line();
}
