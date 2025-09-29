#include "PIT.h"
#include "io.h"
#include "debug.h"

#define cli() __asm__ volatile ("cli")
#define sti() __asm__ volatile ("sti")

#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PIT_COMMAND_PORT  0x43

#define PIT_CR_CHANNEL0           0b00000000
#define PIT_CR_CHANNEL1           0b01000000
#define PIT_CR_CHANNEL2           0b10000000
// #define PIT_CR_READ_BACK       0b11000000
#define PIT_CR_ACCESS_LATCH_COUNT 0b00000000
#define PIT_CR_ACCESS_LOW_BYTE    0b00010000
#define PIT_CR_ACCESS_HIGH_BYTE   0b00100000
#define PIT_CR_ACCESS_LOW_HIGH    0b00110000
#define PIT_CR_MODE_TERMINAL_INT  0b00000000
#define PIT_CR_MODE_HARD_ONE_SHOT 0b00000010
#define PIT_CR_MODE_RATE_GEN      0b00000100
#define PIT_CR_MODE_SQU_WAVE_GEN  0b00000110
#define PIT_CR_MODE_SOFT_TRIG_SB  0b00001000
#define PIT_CR_MODE_HARD_TRIG_SB  0b00001010

u16 current_reload = 0;

void pit_set_reload(u16 reload) {
  current_reload = reload;
  cli();
  outb(PIT_COMMAND_PORT, PIT_CR_CHANNEL0 | PIT_CR_ACCESS_LOW_HIGH | PIT_CR_MODE_SQU_WAVE_GEN);
  outb(PIT_CHANNEL0_PORT, reload & 0xFF);
  outb(PIT_CHANNEL0_PORT, reload >> 8);
  sti();
}

void pit_sleep(u16 ticks) {
  u16 tmp_reload = current_reload;
  if (current_reload != 0)
    pit_set_reload(0);

  do
    __asm__ volatile ("hlt");
  while (--ticks);

  if (tmp_reload != current_reload)
    pit_set_reload(tmp_reload);
}

u16 read_pit_count(u8 channel) {
  if (channel > 2) return 0xFFFF;
  cli();

  u16 count = 0;

  outb(PIT_COMMAND_PORT, (channel << 6) & 0xFF);

  count = inb(PIT_CHANNEL0_PORT + channel);       // Low byte
  count |= inb(PIT_CHANNEL0_PORT + channel) << 8; // High byte

  sti();
  return count;
}

void set_pit_count(u8 channel, u16 count) {
  if (channel > 2) return;
  cli();

  // outb(PIT_COMMAND_PORT, PIT_CR_ACCESS_LOW_HIGH | ((channel << 6) & 0xFF));
  outb(PIT_CHANNEL0_PORT + channel, count & 0xFF); // Low byte
  outb(PIT_CHANNEL0_PORT + channel, count >> 8);   // High byte

  sti();
  return;
}

