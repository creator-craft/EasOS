#include "types.h"
// #include "interrupts_handlers.h"
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

// extern void IRQ33();
// extern void IRQ_N();

extern u32 isr_stub_table[256];

// void *isr_stub_table[] = {
//   IRQ0, IRQ1, IRQ2, IRQ3, IRQ4, IRQ5, IRQ6, IRQ7, IRQ8, IRQ9, IRQ10, IRQ11, IRQ12, IRQ13, IRQ14, IRQ15,
//   IRQ16, IRQ17, IRQ18, IRQ19, IRQ20, IRQ21, IRQ22, IRQ23, IRQ24, IRQ25, IRQ26, IRQ27, IRQ28, IRQ29, IRQ30, IRQ31,
//   IRQ32, IRQ33, IRQ34, IRQ35, IRQ36, IRQ37, IRQ38, IRQ39, IRQ40, IRQ41, IRQ42, IRQ43, IRQ44, IRQ45, IRQ46, IRQ47,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
//   IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N, IRQ_N,
// };

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
