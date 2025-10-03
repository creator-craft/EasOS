#include "screen.h"
#include "VBE.h"

u32 *framebuffer;

void init_screen() {
  framebuffer = (u32*)mode_info_block->framebuffer;
}

void fill_screen(u32 color) {
  u32 *fb = framebuffer;
  for (u32 i = 0; i < mode_info_block->width * mode_info_block->height; i++)
    *(fb++) = color;
}

void draw_image_at(struct image img, u16 x, u16 y) {
  if (x + img.width >= mode_info_block->width || y + img.height >= mode_info_block->height)
    return;

  u32 *fb = framebuffer + (y * mode_info_block->width + x), *pixels = img.pixels;
  for (u32 j = 0; j < img.height; j++) {
    for (u32 i = 0; i < img.width; i++) {
      *(fb++) = *(pixels++);
    }
    fb += mode_info_block->width - img.width;
  }
}

void draw_scaled_image_at(struct image img, u16 x, u16 y, u8 scale) {
  if (x + img.width * scale >= mode_info_block->width || y + img.height * scale >= mode_info_block->height)
    return;

  u32 *fb = framebuffer + (y * mode_info_block->width + x), *pixels = img.pixels;
  for (u32 j = 0; j < img.height; j++) {
    for (u32 r2 = 0; r2 < scale; r2++) {
      for (u32 i = 0; i < img.width; i++) {
        for (u32 r = 0; r < scale; r++) {
          *(fb++) = *pixels;
        }
        pixels++;
      }
      pixels -= img.width;
      fb += mode_info_block->width - img.width * scale;
    }
    pixels += img.width;
  }
}
