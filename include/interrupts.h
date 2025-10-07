#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"

enum PIC_RANGE_0 {
  PIC_PIT      = 0b11111110,
  PIC_KEYBOARD = 0b11111101,
  PIC_CASCADE  = 0b11111011,
  PIC_COM2     = 0b11110111,
  PIC_COM1     = 0b11101111,
  PIC_LPT2     = 0b11011111,
  PIC_FLOPPY   = 0b10111111,
  PIC_LPT1     = 0b01111111
};

enum PIC_RANGE_1 {
  PIC_RTC   = 0b11111110,
  PIC_MOUSE = 0b11101111,
  PIC_FPU   = 0b11011111,
  PIC_ATA1  = 0b10111111,
  PIC_ATA2  = 0b01111111
};

void init_idt();

void map_PIC();
void set_PIC_mask(u8 primary, u8 secondary);
u16 get_PIC_mask();
void wait_interrupt(u8 primary, u8 secondary);


#endif // INTERRUPTS_H