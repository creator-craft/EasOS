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

void hdc1_handler() {
  print("HDC1\n");
}

void hdc2_handler() {
  print("HDC2\n");
}