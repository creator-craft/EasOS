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

  u32 *fb = framebuffer + (y * mode_info_block->width + x);
  const u32 *pixels = img.pixels;
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

  u32 *fb = framebuffer + (y * mode_info_block->width + x);
  const u32 *pixels = img.pixels;
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

inline __attribute__((always_inline)) i32 clamp(i32 v, i32 mn, i32 mx) {
  return v < mn ? mn : v > mx ? mx : v;
}

// EFLA by Po-Han Lin (src: "http://elynxsdk.free.fr/ext-docs/Rasterization/Lines/Lines%20algorithms.htm")
// Upgraded by creator-craft
void draw_line(u32 color, i32 x1, i32 y1, i32 x2, i32 y2) {
  x1 = clamp(x1, 0, mode_info_block->width - 1);
  y1 = clamp(y1, 0, mode_info_block->height - 1);
  x2 = clamp(x2, 0, mode_info_block->width - 1);
  y2 = clamp(y2, 0, mode_info_block->height - 1);

  if (x1 > x2) {
    u16 tmp = x1;
    x1 = x2;
    x2 = tmp;
  }

  if (y1 > y2) {
    u16 tmp = y1;
    y1 = y2;
    y2 = tmp;
  }

  i32 shortLen = y2 - y1;
  i32 longLen = x2 - x1;
  u8 yLonger = shortLen > longLen;

  if (yLonger) {
    i32 swap = shortLen;
    shortLen = longLen;
    longLen = swap;
  }

  i32 decInc = 0;
  if (longLen != 0)
    decInc = (shortLen << 16) / longLen;

  i32 j = 0;
  u32 *fb = framebuffer + (y1 * mode_info_block->width + x1);

  if (yLonger)
    for (i32 i = 0; i != longLen; i++, fb += mode_info_block->width) {
      fb[(j >> 16)] = color;
      j += decInc;
    }
  else
    for (i32 i = 0;i != longLen; i++, fb++) {
      fb[(j >> 16) * mode_info_block->width] = color;
      j += decInc;
    }
}

void draw_horieontal_line(u32 color, u16 x, u16 size, u16 y) {
  if (x + size >= mode_info_block->width || y >= mode_info_block->height)
    return;

  u32 *fb = framebuffer + (y * mode_info_block->width + x);
  for (u32 i = 0; i < size; i++) {
    *(fb++) = color;
  }
}

void draw_vertical_line(u32 color, u16 x, u16 y, u16 size) {
  if (x >= mode_info_block->width || y + size >= mode_info_block->height)
    return;

  u32 *fb = framebuffer + (y * mode_info_block->width + x);
  for (u32 i = 0; i < size; i++) {
    *fb = color;
    fb += mode_info_block->width;
  }
}

void fill_rect(u32 color, u16 x, u16 y, u16 width, u16 height) {
  if (x + width >= mode_info_block->width || y + height >= mode_info_block->height)
    return;

  u32 *fb = framebuffer + (y * mode_info_block->width + x);
  for (u32 j = 0; j < height; j++) {
    for (u32 i = 0; i < width; i++) {
      *(fb++) = color;
    }
    fb += mode_info_block->width - width;
  }
}

void draw_char(u32 color, u16 x, u16 y, u8 chr, struct font font) {
  const u8 *ft_ptr = font.mask + chr * font.height;
  u32 *fb = framebuffer + (y * mode_info_block->width + x);

  for (u32 j = 0; j < font.height; j++) {
    u8 mask = ft_ptr[j];
    for (u32 i = 0; i < 8; i++) {
      if (mask & (0b10000000 >> i))
        fb[i] = color;
    }
    fb += mode_info_block->width;
  }
}

void draw_string(u32 color, u16 x, u16 y, const u8 *str, struct font font) {
  for (u32 i = 0; *str; i++, str++) {
    draw_char(color, x + i * 8, y, *str, font);
  }
}
