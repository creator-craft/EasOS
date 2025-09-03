#include "text_mode.h"
#include "io.h"

#define VGA_ADDRESS (void*)0x000B8000
#define VGA_COMMAND_PORT 0x03D4
#define VGA_DATA_PORT 0x03D5

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_LIMIT SCREEN_HEIGHT * SCREEN_WIDTH

short *text_screen_buffer = VGA_ADDRESS;

struct {
  short offset;
  char info, shape, color;
} cursor = { 0, 0, 0, (BLACK << 4) | WHITE };

void set_cursor_offset(short off) {
  outb(VGA_COMMAND_PORT, 0x0F); // Low byte
  outb(VGA_DATA_PORT, off & 0xFF);

  outb(VGA_COMMAND_PORT, 0x0E); // High byte
  outb(VGA_DATA_PORT, off >> 8);

  cursor.offset = off;
}

// void set_cursor_pos(short x, short y) {
//   set_cursor_offset(y * SCREEN_WIDTH + x);
// }

void set_cursor_shape(char start_line, char end_line) {
  outb(VGA_COMMAND_PORT, 0x0A);
  outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0b11100000) | start_line);

  outb(VGA_COMMAND_PORT, 0x0B);
  outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0b11100000) | end_line);
}


void print(const char *txt) {
  int i;
  for (i = 0; txt[i] && i < SCREEN_LIMIT; i++)
    text_screen_buffer[cursor.offset + i] = (cursor.color << 8) | txt[i];
  set_cursor_offset(cursor.offset + i);
}

void print_text_only(const char *txt) {
  int i;
  for (i = 0; txt[i] && i < SCREEN_LIMIT; i++)
    *((char*)text_screen_buffer + 2 * i) = txt[i];
  set_cursor_offset(cursor.offset + i);
}

void print_new_line() {
  set_cursor_offset(cursor.offset - (cursor.offset % SCREEN_HEIGHT) + SCREEN_WIDTH);

  if (cursor.offset >= SCREEN_LIMIT)
    set_cursor_offset(0);
}

void print_char(char chr) {
  text_screen_buffer[cursor.offset] = (cursor.color << 8) | chr;
  set_cursor_offset(cursor.offset + 1);
}

void remove_char() {
  set_cursor_offset(cursor.offset + 1);
  text_screen_buffer[cursor.offset] = (cursor.color << 8) | ' ';
}

void fill_screen(short el) {
  for (int i = 0; i < SCREEN_LIMIT; i++)
    text_screen_buffer[i] = el;
}

void print_hex_b(char e) {
  char tmp = e & 0xF;
  text_screen_buffer[cursor.offset + 1] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = e >> 4;
  text_screen_buffer[cursor.offset] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  set_cursor_offset(cursor.offset + 2);
}

void print_hex_w(short e) {
  char tmp = e & 0xF;
  text_screen_buffer[cursor.offset + 3] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = e >> 4;
  text_screen_buffer[cursor.offset + 2] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = (e >> 8) & 0xF;
  text_screen_buffer[cursor.offset + 1] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = e >> 12;
  text_screen_buffer[cursor.offset] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  set_cursor_offset(cursor.offset + 4);
}

void print_hex_d(int e) {
  char tmp = e & 0xF;
  text_screen_buffer[cursor.offset + 7] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = e >> 4;
  text_screen_buffer[cursor.offset + 6] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = (e >> 8) & 0xF;
  text_screen_buffer[cursor.offset + 5] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = (e >> 12) & 0xF;
  text_screen_buffer[cursor.offset + 4] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = (e >> 16) & 0xF;
  text_screen_buffer[cursor.offset + 3] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = (e >> 20) & 0xF;
  text_screen_buffer[cursor.offset + 2] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = (e >> 24) & 0xF;
  text_screen_buffer[cursor.offset + 1] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  tmp = e >> 28;
  text_screen_buffer[cursor.offset] = (cursor.color << 8) | ((tmp >= 10 ? 'A' - 10 : '0') + tmp);
  set_cursor_offset(cursor.offset + 8);
}

// short get_cursor_offset() {
//   return cursor.offset;
// }