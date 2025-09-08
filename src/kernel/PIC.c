#include "io.h"

#define ICW_1 0x11      // 00010001 binary. Enables initialization mode and we are sending ICW 4
#define PIC_1_CTRL 0x20 // Primary PIC control register
#define PIC_1_DATA 0x21 // Primary PIC data register
#define PIC_2_CTRL 0xA0 // Secondary PIC control register
#define PIC_2_DATA 0xA1 // Secondary PIC data register

#define IRQ_0_OFF 0x20 // IRQs 0-7 mapped to use interrupts 0x20-0x27
#define IRQ_8_OFF 0x28 // IRQs 8-15 mapped to use interrupts 0x28-0x2F

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

void set_PIC_mask(u8 primary, u8 secondary) {
  outb(PIC_1_DATA, primary);
  outb(PIC_2_DATA, secondary);
}
