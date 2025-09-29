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
