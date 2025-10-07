#include "io.h"

#define ICW_1      0x11 // 00010001 binary. Enables initialization mode and we are sending ICW 4
#define PIC_1_CTRL 0x20 // Primary PIC control register
#define PIC_1_DATA 0x21 // Primary PIC data register
#define PIC_2_CTRL 0xA0 // Secondary PIC control register
#define PIC_2_DATA 0xA1 // Secondary PIC data register

#define IRQ_0_OFF 0x20  // IRQs 0-7 mapped to use interrupts 0x20-0x27
#define IRQ_8_OFF 0x28  // IRQs 8-15 mapped to use interrupts 0x28-0x2F

u16 PIC_mask = 0;

void map_PIC() {
  outb(PIC_1_CTRL, ICW_1);
  outb(PIC_2_CTRL, ICW_1);

  outb(PIC_1_DATA, IRQ_0_OFF);
  outb(PIC_2_DATA, IRQ_8_OFF);

  outb(PIC_1_DATA, 0x04);
  outb(PIC_2_DATA, 0x02);

  outb(PIC_1_DATA, 1);
  outb(PIC_2_DATA, 1);

  outb(PIC_1_DATA, 0);
  outb(PIC_2_DATA, 0);
}

void set_PIC_mask(u16 mask) {
  outb(PIC_1_DATA, mask & 0xFF);
  outb(PIC_2_DATA, mask >> 8);
  PIC_mask = mask;
}

u16 get_PIC_mask() {
  return PIC_mask;
}

void wait_interrupt(u16 mask) {
  u16 old_mask = PIC_mask;
  set_PIC_mask(mask);

  __asm__ volatile ("hlt");

  set_PIC_mask(old_mask);
}
