#include "interrupts.h"
#include "PCI.h"
#include "ATA.h"
#include "PIT.h"
#include "mouse.h"
#include "tests.h"
#include "VBE.h"
#include "screen.h"
#include "debug.h"
#include "resources.h"

DECL_RES(france_bi);

const char *msg = "Hello from C kernel !\n";

void kernel_main() {
  struct image my_img = { RES(france_bi) + 4, *(u16*)RES(france_bi), *((u16*)(RES(france_bi) + 2)) };

  debug_hex_w(my_img.width);
  debug_char(':');
  debug_hex_w(my_img.height);
  debug_new_line();

  init_screen();
  fill_screen(0x101040);

  draw_scaled_image_at(my_img, 100, 100, 16);

  debug(msg);

  init_idt();

  map_PIC();
  set_PIC_mask(0xFF, 0xFF);

  debug("Mouse init: ");
  debug_hex_b(init_mouse(10, 1));
  debug_new_line();

  set_PIC_mask(PIC_CASCADE & PIC_KEYBOARD & PIC_PIT, PIC_ATA1 & PIC_MOUSE);

  test_sleep();

  test_procs();

  test_pci();

  debug_hex_b(identify(0, NULL));
  debug_new_line();
}
