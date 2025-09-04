#include "text_mode.h"
#include "interrupts.h"

const char *msg = "Hello from C kernel !";

void kernel_main() {
  clear_screen();

  print(msg);

  // init_idt(); // Work in progress

}
