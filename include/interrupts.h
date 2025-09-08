#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"

void init_idt();

void map_PIC();
void set_PIC_mask(u8 primary, u8 secondary);

#endif // INTERRUPTS_H