#include "text_mode.h"
#include "interrupts.h"

const char *msg = "Hello from C kernel !";

void kernel_main() {
  clear_screen();

  print(msg);

  init_idt();

  map_PIC();
  set_PIC_mask(0x01, 0x00);

}
