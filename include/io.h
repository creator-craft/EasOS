#ifndef IO_H
#define IO_H

#include "types.h"

static inline void outb(u16 port, u8 val) {
  __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline u8 inb(u16 port) {
  char ret;
  __asm__ volatile ( "inb %1, %0"
                      : "=a"(ret)
                      : "Nd"(port) );
  return ret;
}

#endif // IO_H