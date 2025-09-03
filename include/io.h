#ifndef IO_H
#define IO_H

static inline void outb(short port, char val) {
  __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline char inb(short port) {
  char ret;
  __asm__ volatile ( "inb %1, %0"
                      : "=a"(ret)
                      : "Nd"(port) );
  return ret;
}

#endif // IO_H