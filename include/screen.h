#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"

struct image {
  u32 *pixels;
  u16 width, height;
};

struct font {
  const u8 *mask;
  u8 height;
};

void init_screen();
void update_screen();
void fill_screen(u32 color);
void blit(struct image src, struct image dst, u16 x, u16 y);
void transparent_blit(struct image src, struct image dst, u16 x, u16 y);
void blit_part(struct image src, struct image dst, u16 src_x, u16 src_y, u16 dst_x, u16 dst_y, u16 width, u16 height);
void transparent_blit_part(struct image src, struct image dst, u16 src_x, u16 src_y, u16 dst_x, u16 dst_y, u16 width, u16 height);
void draw_image_at(struct image img, u16 x, u16 y);
void draw_scaled_image_at(struct image img, u16 x, u16 y, u8 scale);
void draw_line(u32 color, i32 x1, i32 y1, i32 x2, i32 y2);
void draw_horieontal_line(u32 color, u16 x, u16 size, u16 y);
void draw_vertical_line(u32 color, u16 x, u16 y, u16 size);
void fill_rect(u32 color, u16 x, u16 y, u16 width, u16 height);
void draw_char(u32 color, u16 x, u16 y, u8 chr, struct font font);
void draw_string(u32 color, u16 x, u16 y, const char *str, struct font font);

inline void draw_rect(u32 color, u16 x, u16 y, u16 width, u16 height) {
  draw_horieontal_line(color, x, width, y);
  draw_horieontal_line(color, x, width, y + height);
  draw_vertical_line(color, x, y, height);
  draw_vertical_line(color, x + width, y, height);
}

#endif