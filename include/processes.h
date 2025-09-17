#ifndef PROCESSES_H
#define PROCESSES_H

#include "types.h"

u8 create_process(void *func, void *stack);
u8 kill_process(u8 pid);

#endif // PROCESSES_H