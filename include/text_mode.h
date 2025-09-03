#ifndef TEXT_MODE_H
#define TEXT_MODE_H

enum color {
  BLACK = 0, BLUE,       GREEN,       CYAN,       RED,       MAGENTA,       BROWN,  LIGHT_GRAY,
  DARK_GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED, LIGHT_MAGENTA, YELLOW, WHITE
};

void set_cursor_offset(short off);
void set_cursor_shape(char start_line, char end_line);

void print(const char *txt);
void print_text_only(const char *txt);
void print_char(char chr);
void remove_char();
void fill_screen(short el);
void print_hex_b(char e);
void print_hex_w(short e);
void print_hex_d(int e);
void print_new_line();


static inline void enable_cursor() { set_cursor_shape(0, 0); }
static inline void println(const char *txt) { print(txt); print_new_line(); };

// inline short get_cursor_offset();
// inline void set_cursor_pos(short x, short y);
// static inline struct {int x, y;} get_cursor_position();

#endif // TEXT_MODE_H