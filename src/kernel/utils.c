#include "utils.h"
#include "debug.h"

void mem_copy(void *dst, const void *src, const u32 count) {
  for (u32 i = 0; i < count / 4; i++, dst += 4, src += 4) {
    *(u32*)dst = *(u32*)src;
  }

  for (u32 i = 0; i < (count & 0b11); i++)
    *(u8*)dst++ = *(u8*)src++;
}

