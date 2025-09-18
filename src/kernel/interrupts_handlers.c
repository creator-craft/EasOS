#include "text_mode.h"
#include "io.h"
#include "keyboard.h"

void keyboard_handler() {
  u8 keycode = inb(0x60);

  u8 scancode = *((u8*)keymap + 4*(keycode & 0b01111111) + key_modifiers);

  if (keycode & 0b10000000) // Key released
    switch (scancode) {
    case KEY_LSHIFT: case KEY_RSHIFT:
      key_modifiers &= ~SHIFT_MODIFIER;
      break;
    case KEY_ALT: case KEY_ALT_SYSREQ:
      key_modifiers &= ~ALT_MODIFIER;
      break;
    case KEY_CTRL:
      key_modifiers &= ~CTRL_MODIFIER;
      break;

    }
  else // Key pressed
    switch (scancode) {
    case KEY_NULL: break;
    case KEY_LSHIFT: case KEY_RSHIFT:
      key_modifiers |= SHIFT_MODIFIER;
      break;
    case KEY_ALT: case KEY_ALT_SYSREQ:
      key_modifiers |= ALT_MODIFIER;
      break;
    case KEY_CTRL:
      key_modifiers |= CTRL_MODIFIER;
      break;
    case KEY_ENTER:
      print_new_line();
      break;
    case KEY_BACKSPACE:
      remove_char();
      break;

    default:
      print_char(scancode);
    }
}

struct {
  u8 mouse_state;
  u8 x_movement;
  u8 y_movement;
  union {
    u8 z_movement;
    u8 extra_state;
  };
} __attribute__((packed)) mouse_packets;

u8 packet_id = 0, mouse_info = 0;
void mouse_handler() {
  if (packet_id == 0) {
    mouse_packets.mouse_state = inb(0x60);
    return;
  } else if (packet_id == 1) {
    mouse_packets.x_movement = inb(0x60);
    return;
  } else if (packet_id == 2) {
    mouse_packets.x_movement = inb(0x60);
    return;
  } else if (packet_id == 3) {
    mouse_packets.x_movement = inb(0x60);
    if (mouse_info == 1)
      return;
  } else
    mouse_packets.extra_state = inb(0x60);

  // Mouse packets ready
}

void hdc1_handler() {
  print("HDC1\n");
}

void hdc2_handler() {
  print("HDC2\n");
}

void test_handler() {
  print_char('-');
}