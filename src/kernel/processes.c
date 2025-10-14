#include "processes.h"
#include "debug.h"

#define EFLAGS_IF  (1 << 9)
#define EFLAGS_MBS (1 << 1)

#define PROCESS_STACK(i) ((u32*)processes_registers[i].esp)

struct process processes[256];
struct process_registers processes_registers[256];

void init_process0() {
  processes[0] = (struct process) { 0, 0, 0, 1 };
}

u8 create_process(void *func, void *stack) {
  for (u32 i = 0; i < 256; i++)
    if (processes[i].state == 0) {
      processes[i] = (struct process) { i, 0, 0, 1 };
      processes_registers[i].esp = (u32)stack - 12;
      PROCESS_STACK(i)[0] = func; // EIP
      PROCESS_STACK(i)[1] = 0x00000008; // CS
      PROCESS_STACK(i)[2] = EFLAGS_IF | EFLAGS_MBS; // EFLAG

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

u8 process_call(u8 pid, void *function) {
  if (processes[pid].state == 0)
    return 0;

  // Critical section
  __asm__ volatile ("cli");
  processes_registers[pid].esp -= 4; // TODO: use general expand function

  u32 process_IP = PROCESS_STACK(pid)[1];
  PROCESS_STACK(pid)[1] = PROCESS_STACK(pid)[2]; // Rearange iret destination info
  PROCESS_STACK(pid)[2] = PROCESS_STACK(pid)[3];
  PROCESS_STACK(pid)[3] = process_IP; // Old EIP => simple ret EIP

  PROCESS_STACK(pid)[0] = (u32)function;
  __asm__ volatile ("sti");

  return 1;
}