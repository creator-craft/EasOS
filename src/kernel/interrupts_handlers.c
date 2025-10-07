#include "debug.h"
#include "io.h"
#include "keyboard.h"
#include "screen.h"
#include "resources.h"

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
      debug_new_line();
      break;
    case KEY_BACKSPACE:
      // remove_char();
      break;

    default:
      debug_char(scancode);
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

u16 cursor_x = 0, cursor_y = 0;
u8 packet_id = 0, mouse_info = 0;
void mouse_handler() {
  u8 status = inb(0x64);
  if (!(status & 0x20)) {
    debug("Mouse but key...\n");
    return;
  }

  if (packet_id == 0) {
    mouse_packets.mouse_state = inb(0x60);
    packet_id++;
    return;
  } else if (packet_id == 1) {
    mouse_packets.x_movement = inb(0x60);
    packet_id++;
    return;
  } else if (packet_id == 2) {
    mouse_packets.y_movement = inb(0x60);
    packet_id++;
    if (mouse_info == 1)
      return;
  } else/* if (packet_id == 3)*/ {
    mouse_packets.extra_state = inb(0x60);
    packet_id++;
  }

  packet_id = 0;

  fill_rect(0x000000, cursor_x, cursor_y, cursor.width, cursor.height);

  i16 dx = (mouse_packets.mouse_state & 0b010000 ? (0xFF00 | mouse_packets.x_movement) : mouse_packets.x_movement);
  i16 dy = (mouse_packets.mouse_state & 0b100000 ? (0xFF00 | mouse_packets.y_movement) : mouse_packets.y_movement);

  cursor_x += dx;
  cursor_y -= dy;

  draw_image_at(cursor, cursor_x, cursor_y);
}

void hdc1_handler() {
  debug("HDC1\n");
}

void hdc2_handler() {
  debug("HDC2\n");
}

void test_handler() {
  debug_char('-');
}