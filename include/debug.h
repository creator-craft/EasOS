#ifndef DEBUG_H
#define DEBUG_H

#include "types.h"
#include "io.h"

void debug_hex_d(u32 value);
void debug_hex_w(u16 value);
void debug_hex_b(u8 value);
void debug(char* v);

static inline __attribute__((always_inline)) void debug_char(char v) {
  outb(0x03F8, v);
}

static inline __attribute__((always_inline)) void debug_new_line() {
  outb(0x03F8, '\n');
}

#endif