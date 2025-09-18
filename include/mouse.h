#ifndef MOUSE_H
#define MOUSE_H

#include "types.h"

u8 init_mouse(u8 sample_rate, u8 resolution);
u8 mouse_enable_scroll_wheel();

#endif // MOUSE_H