#include "text_mode.h"

const char *msg = "Hello from C kernel !";

void kernel_main() {
  fill_screen(' ');

  print(msg);

}
