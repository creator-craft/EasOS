#include "text_mode.h"
#include "io.h"

void keyboard_handler() {
  print_char(inb(0x60));
}
