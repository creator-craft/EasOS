#include "resources.h"

DECL_RES(mouse_bi)

struct image cursor;

void init_resources() {
  cursor = (struct image) { (const u32*)RES(mouse_bi) + 1, *(u16*)RES(mouse_bi), *((u16*)(RES(mouse_bi) + 2)) };
}
