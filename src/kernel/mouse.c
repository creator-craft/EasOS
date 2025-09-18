#include "io.h"

#define READ_OPERATION  1
#define WRITE_OPERATION 2

void mouse_wait(u8 ope) {
  for (u32 i = 0; i < 10000; i++)
    if (inb(0x64) & ope)
      return;
}

void mouse_write(u8 val) {
  mouse_wait(WRITE_OPERATION);
  outb(0x64, 0xD4);
  mouse_wait(WRITE_OPERATION);
  outb(0x60, val);
  mouse_wait_ACK();
}

void mouse_wait_ACK() {
  for (u32 i = 0; i < 60000; i++)
    if (inb(0x60) == 0xFA)
      return;
}

void init_mouse(u8 sample_rate, u8 resolution) { // Default: 10, 0x01
  mouse_wait(WRITE_OPERATION);
  outb(0x64, 0xA8); // Enable Mouse device

  mouse_wait(WRITE_OPERATION);
  outb(0x64, 0x20); // Enable mouse's interrupt
  mouse_wait(READ_OPERATION);
  u8 tmp = inb(0x60) | 0x02;
  mouse_wait(WRITE_OPERATION);
  outb(0x64, 0x60);
  mouse_wait(WRITE_OPERATION);
  outb(0x60, tmp);

  mouse_write(0xFF);        // Reset mouse
  mouse_write(0xF6);        // Use defaults
  mouse_write(0xF3);        // Set sample rate
  mouse_write(sample_rate); // Sample rate
  mouse_write(0xE8);        // Set resolution
  mouse_write(resolution);  // Resolution
  mouse_write(0xF4);        // Enable packed streaming

}

// TODO: Check if it works well & activate 4th mouse's byte
