#include "debug.h"

#define COM_PORT 0x03F8

void debug_hex_d(u32 v) {
  u8 tmp = v >> 28;
  outb(COM_PORT, (tmp >= 0xA ? 'A'  -  10 : '0') + tmp);
  tmp = (v >> 24) & 0xF;
  outb(COM_PORT, (tmp >= 0xA ? 'A'  -  10 : '0') + tmp);
  tmp = (v >> 20) & 0xF;
  outb(COM_PORT, (tmp >= 0xA ? 'A'  -  10 : '0') + tmp);
  tmp = (v >> 16) & 0xF;
  outb(COM_PORT, (tmp >= 0xA ? 'A'  -  10 : '0') + tmp);
  tmp = (v >> 12) & 0xF;
  outb(COM_PORT, (tmp >= 0xA ? 'A' -  10 : '0') + tmp);
  tmp = (v >> 8) & 0xF;
  outb(COM_PORT, (tmp >= 0xA ? 'A' -  10 : '0') + tmp);
  tmp = (v >> 4) & 0xF;
  outb(COM_PORT, (tmp >= 0xA ? 'A' -  10 : '0') + tmp);
  tmp = v & 0xF;
  outb(COM_PORT, (tmp >= 0xA ? 'A' -  10 : '0') + tmp);
}

void debug_hex_w(u16 v) {
  u8 tmp = v >> 12;
  outb(COM_PORT, (tmp >= 0xA ? 'A'  -  10 : '0') + tmp);
  tmp = (v >> 8) & 0xF;
  outb(COM_PORT, (tmp >= 0xA ? 'A'  -  10 : '0') + tmp);
  tmp = (v >> 4) & 0xF;
  outb(COM_PORT, (tmp >= 0xA ? 'A'  -  10 : '0') + tmp);
  tmp = v & 0xF;
  outb(COM_PORT, (tmp >= 0xA ? 'A'  -  10 : '0') + tmp);
}

void debug_hex_b(u8 v) {
  outb(COM_PORT, (v >= 0xA0 ? 'A'  -  10 : '0') + (v >> 4));
  outb(COM_PORT, ((v & 0xF) >= 0xA ? 'A'  -  10 : '0') + (v & 0xF));
}

void debug(const char *str) {
  while (*str)
    outb(COM_PORT, *(str++));
}
