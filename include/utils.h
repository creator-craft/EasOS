#ifndef UTILS_H
#define UTILS_H

#include "types.h"


#define PUSHA() __asm__ volatile ("pushal")
#define POPA() __asm__ volatile ("popal")
#define STI() __asm__ volatile ("sti")
#define CLI() __asm__ volatile ("cli")

extern void small_copy(void *dst, const void *beg, const u32 count);
extern void medium_mem_copy(void *dst, const void *beg, const u32 count);
extern void large_mem_copy(void *dst, const void *beg, const u32 count);
extern void scaled_mem_copy(void *dst, const void *beg, const u32 count, const u32 scale);

#define CLAMP(v, mn, mx) (v < mn ? mn : v > mx ? mx : v)
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a < b ? b : a)

inline __attribute__((always_inline)) i32 clamp(i32 v, i32 mn, i32 mx) {
  return v < mn ? mn : v > mx ? mx : v;
}


#endif // UTILS_H