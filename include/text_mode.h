#ifndef TEXT_MODE_H
#define TEXT_MODE_H

#include "types.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_LIMIT SCREEN_HEIGHT * SCREEN_WIDTH

enum color {
  BLACK = 0, BLUE,       GREEN,       CYAN,       RED,       MAGENTA,       BROWN,  LIGHT_GRAY,
  DARK_GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED, LIGHT_MAGENTA, YELLOW, WHITE
};

void set_cursor_offset(u16 off);
void set_cursor_shape(u8 start_line, u8 end_line);

void print(const char *txt);
void print_text_only(const char *txt);
void print_char(char chr);
void remove_char();
void fill_screen(u16 el);
void print_hex_b(u8 e);
void print_hex_w(u16 e);
void print_hex_d(u32 e);
void print_new_line();
short get_cursor_offset();


static inline void enable_cursor() { set_cursor_shape(0, 0); }
static inline void println(const char *txt) { print(txt); print_new_line(); }
static inline void clear_screen() { fill_screen(0x0F00 | ' '); }
inline void set_cursor_pos(u16 x, u16 y) { set_cursor_offset(y * SCREEN_WIDTH + x); }

// static inline struct {int x, y;} get_cursor_position();

#endif // TEXT_MODE_H