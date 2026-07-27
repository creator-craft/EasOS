#include "input.h"
#include "debug.h"

u8 keyboard_priority_input = UNDEFINED_INPUT, mouse_priority_input = UNDEFINED_INPUT;
struct input processes_inputs[INPUTS_COUNT];

void addPacket(u32 input_idx, struct input_packet packet, u8 input_type) {
  struct input *process_input = &processes_inputs[input_idx];
  struct process *proc = &processes[process_input->pid];
  if (proc->flags & input_type) {
    process_input->packets[(process_input->packets_index + process_input->packets_count) % PACKETS_MAX_COUNT] = packet;

    if (process_input->packets_count < PACKETS_MAX_COUNT)
      process_input->packets_count ++;
    else {
      process_input->packets_index ++;
      if (process_input->packets_index >= PACKETS_MAX_COUNT)
        process_input->packets_index = 0;
    }

    debug_hex_b(process_input->pid);
    debug("NOP");
    // Awake process if it sleep
    if (proc->flags & PFLAG_WAIT_FOR_INPUT)
      proc->state = RUNNABLE;
  }
}
