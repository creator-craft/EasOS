#ifndef PROCESSES_H
#define PROCESSES_H

#include "types.h"

u8 create_process(void *func, void *stack);
u8 kill_process(u8 pid);

struct process {
  u8 pid, cycles, flags, state;
  u8 reserved[4];
} __attribute__ ((packed));

struct process_registers {
  u32 eax, ebx, ecx, edx, ebp, esi, edi, esp;
} __attribute__ ((packed));

extern struct process processes[];
extern struct process_registers processes_registers[];

void init_process0();

#endif // PROCESSES_H