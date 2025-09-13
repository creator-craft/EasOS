#ifndef PIT_H
#define PIT_H

#include "types.h"

void pit_sleep(u16 ticks);
void pit_set_reload(u16 reload);

u16 read_pit_count(u8 channel);
void set_pit_count(u8 channel, u16 count);

#endif // PIT_H