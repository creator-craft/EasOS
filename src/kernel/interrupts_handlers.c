#include "debug.h"
#include "io.h"
#include "keyboard.h"
#include "screen.h"
#include "resources.h"
#include "VBE.h"
#include "utils.h"
#include "input.h"
#include "processes.h"

void keyboard_handler() {
  u8 keycode = inb(0x60);
  // if (keycode == 0xFA) return;

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
    case KEY_LSHIFT: case KEY_RSHIFT:
      key_modifiers |= SHIFT_MODIFIER;
      break;
    case KEY_ALT: case KEY_ALT_SYSREQ:
      key_modifiers |= ALT_MODIFIER;
      break;
    case KEY_CTRL:
      key_modifiers |= CTRL_MODIFIER;
      break;
    }

  struct input_packet packet = { .keyboard = {
    .scancode = keycode,
    .chr = scancode,
    .input_type = KEYBOARD_TYPE,
    .key_flags = key_modifiers | (keycode & 0b10000000)
  }, .timestamp = ticks };

  addEvent(packet, PFLAG_KEYBOARD_INPUT);
}

u16 cursor_x = 0, cursor_y = 0;
struct mouse_packet_info mouse_packets;

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

  struct input_packet mouse_packet = { .mouse = mouse_packets, .timestamp = ticks };

  addEvent(mouse_packet, PFLAG_MOUSE_INPUT);

  i16 dx = (mouse_packets.mouse_state & 0b010000 ? (0xFF00 | mouse_packets.x_movement) : mouse_packets.x_movement);
  i16 dy = (mouse_packets.mouse_state & 0b100000 ? (0xFF00 | mouse_packets.y_movement) : mouse_packets.y_movement);

  cursor_x += dx;
  cursor_y -= dy;

  if (cursor_x > mode_info_block->width - 17)
    cursor_x = dx < 0 ? 0 : mode_info_block->width - 17;

  if (cursor_y > mode_info_block->height - 17)
    cursor_y = dy >= 0 ? 0 : mode_info_block->height - 17;

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