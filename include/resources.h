#ifndef EXTERNAL_H
#define EXTERNAL_H

#include "types.h"

#define DECL_RES(x) extern const u8 _binary_res_##x##_start[]; extern const u8 _binary_res_##x##_end[];

#define RES(x) _binary_res_##x##_start
#define EXT_REs(x) _binary_res_##x##_end
#define RES_SIZE(x) (RES_END(x) - RES(x))

#endif // EXTERNAL_H