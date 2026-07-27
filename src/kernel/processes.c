#include "processes.h"
#include "debug.h"
#include "utils.h"
#include "input.h"

#define EFLAGS_IF  (1 << 9)
#define EFLAGS_MBS (1 << 1)

#define PROCESS_STACK(i) ((u32*)processes[i].registers.esp)
extern void kernel_clock();

/* Sizes and stack layout */
#define IDLE_STACK_TOP 0x9FC0
#define STACK_FRAME_BYTES 12

struct process processes[PROCESS_COUNT];

volatile u32 ticks = 0;
volatile u8 current_process_id = 0;

u32 clock() {
  ticks ++;

  /* Find next RUNNABLE process (bounded search). */
  u8 pid = current_process_id;
  do {
    pid++;
    /* wrap-around handled by u8 overflow */
    if (processes[pid].state == RUNNABLE) {
      current_process_id = pid;
      break;
    }

  } while (pid != current_process_id);

  kernel_clock();

  if (processes[pid].state == SLEEP)
    return IDLE_PROCESS_ID;

  return pid;
}

void empty_process() {
  while (1);
}

void init_processes() {
  for (int i = 0; i < PROCESS_COUNT; i++)
    processes[i].state = STOPPED;

  // Kernel
  processes[0] = (struct process) { {}, 0, 0, 0, RUNNABLE, UNDEFINED_INPUT, {} };

  // Idle
  processes[IDLE_PROCESS_ID] = (struct process) { {}, IDLE_PROCESS_ID, 0, 0, SLEEP, UNDEFINED_INPUT, {} };
  processes[IDLE_PROCESS_ID].registers.esp = IDLE_STACK_TOP;
  PROCESS_STACK(IDLE_PROCESS_ID)[0] = (u32)empty_process; // EIP
  PROCESS_STACK(IDLE_PROCESS_ID)[1] = 0x00000008; // CS
  PROCESS_STACK(IDLE_PROCESS_ID)[2] = EFLAGS_IF | EFLAGS_MBS; // EFLAG
}

u8 create_process(void *func, void *stack) {
  CLI();
  for (u32 i = 1; i < PROCESS_COUNT; i++)
    if (processes[i].state == STOPPED) {
      processes[i] = (struct process) { {}, i, 0, 0, RUNNABLE, UNDEFINED_INPUT, {} };
      processes[i].registers.esp = (u32)stack - STACK_FRAME_BYTES;
      PROCESS_STACK(i)[0] = (u32)func; // EIP
      PROCESS_STACK(i)[1] = 0x00000008; // CS
      PROCESS_STACK(i)[2] = EFLAGS_IF | EFLAGS_MBS; // EFLAG
      STI();

      return i & 0xFF;
    }

  STI();
  return 0xFF;
}

u8 kill_process(u8 pid) {
  if (processes[pid].state == STOPPED)
    return 0;
  processes[pid].state = STOPPED; // TODO: Check process IO usage before killing (disk..)
  return 1;
}

u8 process_call(u8 pid, void *function) {
  if (processes[pid].state == STOPPED)
    return 0;

  // Critical section
  CLI();
  processes[pid].registers.esp -= 4; // TODO: use general expand function

  u32 process_IP = PROCESS_STACK(pid)[1];
  PROCESS_STACK(pid)[1] = PROCESS_STACK(pid)[2]; // Rearange iret destination info
  PROCESS_STACK(pid)[2] = PROCESS_STACK(pid)[3];
  PROCESS_STACK(pid)[3] = process_IP; // Old EIP => simple ret EIP

  PROCESS_STACK(pid)[0] = (u32)function;
  STI();

  return 1;
}