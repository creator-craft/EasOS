#ifndef IO_H
#define IO_H

#include "types.h"

static inline __attribute__((always_inline)) void outb(u16 port, u8 val) {
  __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline __attribute__((always_inline)) void outw(u16 port, u16 val) {
  __asm__ volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}

static inline __attribute__((always_inline)) void outd(u16 port, u32 val) {
  __asm__ volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

static inline __attribute__((always_inline)) u8 inb(u16 port) {
  u8 ret;
  __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
  return ret;
}

static inline __attribute__((always_inline)) u16 inw(u16 port) {
  u16 ret;
  __asm__ volatile ( "inw %1, %0" : "=a"(ret) : "Nd"(port) );
  return ret;
}

static inline __attribute__((always_inline)) u32 ind(u16 port) {
  u32 ret;
  __asm__ volatile ( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
  return ret;
}

#endif // IO_H