#include "screen.h"
#include "VBE.h"

u32 *offscreen_buffer, *framebuffer;
u16 frame_width, frame_height;

void init_screen() {
  offscreen_buffer = (u32*)0x100000;
  framebuffer = (u32*)mode_info_block->framebuffer;
  frame_width = mode_info_block->width;
  frame_height = mode_info_block->height;
}

void blit(struct image src, struct image dst, u16 x, u16 y) {
  if (x + src.width >= dst.width || y + src.height >= dst.height)
    return;

  u32 *fb = dst.pixels + (y * dst.width + x);
  const u32 *pixels = src.pixels;
  for (u32 j = 0; j < src.height; j++) {
    for (u32 i = 0; i < src.width; i++) {
      *(fb++) = *(pixels++);
    }
    fb += dst.width - src.width;
  }
}

void transparent_blit(struct image src, struct image dst, u16 x, u16 y) {
  if (x + src.width >= dst.width || y + src.height >= dst.height)
    return;

  u32 *fb = dst.pixels + (y * dst.width + x);
  const u32 *pixels = src.pixels;
  for (u32 j = 0; j < src.height; j++) {
    for (u32 i = 0; i < src.width; i++, fb++, pixels++) {
      if (*pixels)
        *fb = *pixels;
    }
    fb += dst.width - src.width;
  }
}

void blit_part(struct image src, struct image dst, u16 src_x, u16 src_y, u16 dst_x, u16 dst_y, u16 width, u16 height) {
  if (src_x + width >= dst.width || src_y + height >= dst.height || dst_x + width >= dst.width || dst_y + height >= dst.height)
    return;

  u32 *fb = dst.pixels + (dst_y * dst.width + dst_x);
  const u32 *pixels = src.pixels + (src_y * src.width + src_x);
  for (u32 j = 0; j < height; j++) {
    for (u32 i = 0; i < width; i++) {
      *(fb++) = *(pixels++);
    }
    pixels += src.width -  width;
    fb += dst.width - width;
  }
}

void transparent_blit_part(struct image src, struct image dst, u16 src_x, u16 src_y, u16 dst_x, u16 dst_y, u16 width, u16 height) {
  if (src_x + width >= dst.width || src_y + height >= dst.height || dst_x + width >= dst.width || dst_y + height >= dst.height)
    return;

  u32 *fb = dst.pixels + (dst_y * dst.width + dst_x);
  const u32 *pixels = src.pixels + (src_y * src.width + src_x);
  for (u32 j = 0; j < height; j++) {
    for (u32 i = 0; i < width; i++, fb++, pixels++) {
      if (*pixels)
        *fb = *pixels;
    }
    pixels += src.width -  width;
    fb += dst.width - width;
  }
}

void update_screen() {
  u32 *os = offscreen_buffer;
  u32 *fb = framebuffer;
  for (u32 i = 0; i < frame_width * frame_height; i++) {
    *(fb++) = *(os++);
  }
}

void fill_screen(u32 color) {
  u32 *fb = offscreen_buffer;
  for (u32 i = 0; i < frame_width * frame_height; i++)
    *(fb++) = color;
}

void draw_image_at(struct image img, u16 x, u16 y) {
  if (x + img.width >= frame_width || y + img.height >= frame_height)
    return;

  u32 *fb = offscreen_buffer + (y * frame_width + x);
  const u32 *pixels = img.pixels;
  for (u32 j = 0; j < img.height; j++) {
    for (u32 i = 0; i < img.width; i++) {
      *(fb++) = *(pixels++);
    }
    fb += frame_width - img.width;
  }
}

void draw_scaled_image_at(struct image img, u16 x, u16 y, u8 scale) {
  if (x + img.width * scale >= frame_width || y + img.height * scale >= frame_height)
    return;

  u32 *fb = offscreen_buffer + (y * frame_width + x);
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
      fb += frame_width - img.width * scale;
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
  x1 = clamp(x1, 0, frame_width - 1);
  y1 = clamp(y1, 0, frame_height - 1);
  x2 = clamp(x2, 0, frame_width - 1);
  y2 = clamp(y2, 0, frame_height - 1);

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
  u32 *fb = offscreen_buffer + (y1 * frame_width + x1);

  if (yLonger)
    for (i32 i = 0; i != longLen; i++, fb += frame_width) {
      fb[(j >> 16)] = color;
      j += decInc;
    }
  else
    for (i32 i = 0;i != longLen; i++, fb++) {
      fb[(j >> 16) * frame_width] = color;
      j += decInc;
    }
}

void draw_horieontal_line(u32 color, u16 x, u16 size, u16 y) {
  if (x + size >= frame_width || y >= frame_height)
    return;

  u32 *fb = offscreen_buffer + (y * frame_width + x);
  for (u32 i = 0; i < size; i++) {
    *(fb++) = color;
  }
}

void draw_vertical_line(u32 color, u16 x, u16 y, u16 size) {
  if (x >= frame_width || y + size >= frame_height)
    return;

  u32 *fb = offscreen_buffer + (y * frame_width + x);
  for (u32 i = 0; i < size; i++) {
    *fb = color;
    fb += frame_width;
  }
}

void fill_rect(u32 color, u16 x, u16 y, u16 width, u16 height) {
  if (x + width >= frame_width || y + height >= frame_height)
    return;

  u32 *fb = offscreen_buffer + (y * frame_width + x);
  for (u32 j = 0; j < height; j++) {
    for (u32 i = 0; i < width; i++) {
      *(fb++) = color;
    }
    fb += frame_width - width;
  }
}

void draw_char(u32 color, u16 x, u16 y, u8 chr, struct font font) {
  const u8 *ft_ptr = font.mask + chr * font.height;
  u32 *fb = offscreen_buffer + (y * frame_width + x);

  for (u32 j = 0; j < font.height; j++) {
    u8 mask = ft_ptr[j];
    for (u32 i = 0; i < 8; i++) {
      if (mask & (0b10000000 >> i))
        fb[i] = color;
    }
    fb += frame_width;
  }
}

void draw_string(u32 color, u16 x, u16 y, const char *str, struct font font) {
  for (u32 i = 0; *str; i++, str++) {
    draw_char(color, x + i * 8, y, *str, font);
  }
}
