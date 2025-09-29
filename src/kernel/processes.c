#include "processes.h"
#include "debug.h"

#define EFLAGS_IF  (1 << 9)
#define EFLAGS_MBS (1 << 1)

struct process {
  void *stack;
  u8 pid, cycles, res, state;
} __attribute__ ((packed));

struct process processes[256];

u8 create_process(void *func, void *stack) {
  for (u32 i = 0; i < 256; i++)
    if (processes[i].state == 0) {
      processes[i] = (struct process) { stack - 16, i, 0, 0, 1 };
      *(void**)(processes[i].stack + 4) = func; // EIP
      *(u32*)(processes[i].stack + 8) = 0x00000008; // CS
      *(u32*)(processes[i].stack + 12) = EFLAGS_IF | EFLAGS_MBS; // EFLAG

      *(u32*)(processes[i].stack + 0) = 0x00000000; // EFLAG (for popfd)
      processes[i].stack -= 8*4;
      *(u32*)(processes[i].stack + 16) = (u32)processes[i].stack + 8*4; // ESP

      return i & 0xFF;
    }

  return 0xFF;
}

u8 kill_process(u8 pid) {
  if (processes[pid].state == 0)
    return 0;
  processes[pid].state = 0; // TODO: Check process IO usage before killing (disk..)
  return 1;
}