#ifndef INPUT_H
#define INPUT_H

#include "types.h"
#include "processes.h"

#define INPUTS_COUNT 8
#define PACKETS_MAX_COUNT 511 // ?? 2047
#define KEYBOARD_TYPE (1 << 3)
#define MOUSE_TYPE (1 << 3) // TODO: remove
#define UNDEFINED_INPUT 0xFF

enum key_flags {
  KFLAG_PRESSED = 1 << 0,
  KFLAG_SHIFT   = 1 << 1,
  KFLAG_CTRL    = 1 << 2,
  KFLAG_ALT     = 1 << 3,
};

struct mouse_packet_info {
  u8 mouse_state;
  u8 x_movement;
  u8 y_movement;
  union {
    u8 z_movement;
    u8 extra_state;
  };
} __attribute__((packed));

struct input_packet {
  union {
    struct {
      u8 input_type, key_flags, scancode;
      char chr;
    } keyboard;
    struct mouse_packet_info mouse;
  };
  u32 timestamp;
};

struct input {
  u16 packets_count, packets_index;
  u8 pid, reserved[3];
  struct input_packet packets[PACKETS_MAX_COUNT];
};

void addPacket(u32 input_idx, struct input_packet packet, u8 input_type);

extern u8 keyboard_priority_input, mouse_priority_input;
extern struct input processes_inputs[];

inline void __attribute__((always_inline)) addEvent(struct input_packet packet, u8 input_type) {
  u8 priority_input = input_type & PFLAG_MOUSE_INPUT ? mouse_priority_input : input_type & PFLAG_KEYBOARD_INPUT ? keyboard_priority_input : UNDEFINED_INPUT;
  if (priority_input != UNDEFINED_INPUT)
    addPacket(priority_input, packet, input_type);
  else
    for (int i = 0; i < INPUTS_COUNT; i++)
      if (processes_inputs[i].pid != UNDEFINED_INPUT)
        addPacket(i, packet, input_type);
}

#endif // INPUT_H