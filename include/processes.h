#ifndef PROCESSES_H
#define PROCESSES_H

#include "types.h"

enum status {
  STOPPED = 0, RUNNABLE, SLEEP, ZOMBIE
};

enum process_flags {
  SIMD_PROCESS
};

struct process {
  u8 pid, cycles, flags, state;
  u8 reserved[4];
} __attribute__ ((packed));

struct process_registers {
  u32 eax, ebx, ecx, edx, ebp, esi, edi, esp;
} __attribute__ ((packed));

void init_processes();
u8 create_process(void *func, void *stack);
u8 kill_process(u8 pid);
u8 process_call(u8 pid, void *function);

extern struct process processes[];
extern struct process_registers processes_registers[];

#endif // PROCESSES_H