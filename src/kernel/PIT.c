#include "PIT.h"
#include "io.h"
#include "text_mode.h"

#define cli() __asm__ volatile ("cli")
#define sti() __asm__ volatile ("sti")

u16 current_reload = 0;

void pit_set_reload(u16 reload) {
  current_reload = reload;
  cli();
  outb(0x43, 0x36);       // canal 0, low+high, mode 3 (square wave)
  outb(0x40, reload & 0xFF);
  outb(0x40, reload >> 8);
  sti();
}

void pit_sleep(u16 ticks) {
  u16 tmp_reload = current_reload;
  if (current_reload != 0)
    pit_set_reload(0);

  do {
    __asm__ volatile ("hlt");
    // print_char('.');
  } while (--ticks);

  if (tmp_reload != current_reload)
    pit_set_reload(tmp_reload);
}

u16 read_pit_count(u8 channel) {
  if (channel > 2) return 0xFFFF;
  cli();

  u16 count = 0;

  outb(0x43, (channel << 6) & 0xFF);

  count = inb(0x40 + channel);       // Low byte
  count |= inb(0x40 + channel) << 8; // High byte

  sti();
  return count;
}

void set_pit_count(u8 channel, u16 count) {
  if (channel > 2) return;
  cli();

  outb(0x43, 0b00110000 | ((channel << 6) & 0xFF));
  outb(0x40 + channel, count & 0xFF); // Low byte
  outb(0x40 + channel, count >> 8);   // High byte

  sti();
  return;
}

