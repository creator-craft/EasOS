#include "types.h"
#include "interrupts.h"

struct idt_entry {
  u16 handler_low;
  u16 gdt_code_offset;
  u8 reserved;
  u8 flags;
  u16 handler_high;
} __attribute__((packed)) idt_entries[256];

struct {
  u16 limit;
  void *base;
} __attribute__((packed)) idtr = { sizeof(struct idt_entry) * 256 - 1, idt_entries };

extern u32 isr_stub_table[256];

void init_idt() {
  for (u32 i = 0; i < 256; i++) {
    idt_entries[i] = (struct idt_entry) {
      (u32)isr_stub_table[i],
      0x0008, 0x00, 0x8E,
      (u32)isr_stub_table[i] >> 16,
    };
  }

  __asm__ volatile ( "lidt %0" : : "m"(idtr) );
  __asm__ volatile ( "sti" );
}
