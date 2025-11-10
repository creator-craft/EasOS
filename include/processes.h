#ifndef PROCESSES_H
#define PROCESSES_H

#include "types.h"

enum status {
  STOPPED = 0, RUNNABLE, SLEEP, ZOMBIE
};

enum process_flags {
  PFLAG_SIMD_USED      = 1 << 0,
  PFLAG_KEYBOARD_INPUT = 1 << 1,
  PFLAG_MOUSE_INPUT    = 1 << 2,
  PFLAG_WAIT_FOR_INPUT = 1 << 3,
};

struct process {
  struct {
    u32 eax, ebx, ecx, edx, ebp, esi, edi, esp;
  } registers; // 32
  u8 pid, cycles, flags, state;
  u8 input_index;
  u8 reserved[3 + 24];
} __attribute__ ((packed));

void init_processes();
u8 create_process(void *func, void *stack);
u8 kill_process(u8 pid);
u8 process_call(u8 pid, void *function);

extern struct process processes[];
extern u32 ticks;

#endif // PROCESSES_H