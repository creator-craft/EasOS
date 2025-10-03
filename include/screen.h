#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"

struct image {
  u32 *pixels;
  u16 width, height;
};

void init_screen();
void fill_screen(u32 color);
void draw_image_at(struct image img, u16 x, u16 y);
void draw_scaled_image_at(struct image img, u16 x, u16 y, u8 scale);

#endif