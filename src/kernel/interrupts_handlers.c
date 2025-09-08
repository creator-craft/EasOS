#include "text_mode.h"
#include "io.h"

static inline void __attribute__((always_inline)) iret() { __asm__ volatile ( "iret" ); }

void keyboard_handler() {
  print_char(inb(0x60));
}
