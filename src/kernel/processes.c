#include "processes.h"
#include "text_mode.h"

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
      *(void**)(processes[i].stack + 8) = 0x00000008; // CS
      *(void**)(processes[i].stack + 12) = 0x00000000; // EFLAG

      *(void**)(processes[i].stack + 0) = 0x00000000; // EFLAG (for popfd)
      processes[i].stack -= 8*4;
      *(void**)(processes[i].stack + 16) = processes[i].stack + 8*4; // ESP

      return i & 0xFF;
    }

  return 0xFF;
}

