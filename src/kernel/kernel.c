#include "interrupts.h"
#include "PCI.h"
#include "ATA.h"
#include "PIT.h"
#include "mouse.h"
#include "tests.h"
#include "VBE.h"
#include "screen.h"
#include "resources.h"
#include "debug.h"
#include "processes.h"

void kernel_main() {
  init_process0();

  init_screen();
  init_resources();

  init_idt();

  map_PIC();
  set_PIC_mask(PIC_NONE);

  debug("Mouse init: ");
  debug_hex_b(init_mouse(10, 1));
  debug_new_line();

  set_PIC_mask(PIC_CASCADE & PIC_KEYBOARD & PIC_PIT & PIC_ATA1 & PIC_MOUSE);

  tests();

  debug_hex_b(identify(0, NULL));
  debug_new_line();

  while(1) {
    wait_interrupt(PIC_CASCADE & PIC_KEYBOARD & PIC_MOUSE);
  }
}
