#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"

enum PIC_INTERRUPTS {
  PIC_PIT      =         0b11111110,
  PIC_KEYBOARD =         0b11111101,
  PIC_CASCADE  =         0b11111011,
  PIC_COM2     =         0b11110111,
  PIC_COM1     =         0b11101111,
  PIC_LPT2     =         0b11011111,
  PIC_FLOPPY   =         0b10111111,
  PIC_LPT1     =         0b01111111,

  PIC_RTC      = 0b1111111011111111,
  PIC_MOUSE    = 0b1110111111111111,
  PIC_FPU      = 0b1101111111111111,
  PIC_ATA1     = 0b1011111111111111,
  PIC_ATA2     = 0b0111111111111111,

  PIC_NONE     = 0b1111111111111111,
  PIC_ALL      = 0b0000000000000000
};

void init_idt();

void map_PIC();
void set_PIC_mask(u16 mask);
u16 get_PIC_mask();
void wait_interrupt(u16 mask);


#endif // INTERRUPTS_H