#include "common.h"

u32 rand_u32_index = 0;

u32 rand_u32_index_next() {
    u32 index = rand_u32_index;
    if (rand_u32_index + 1 == MAX_U32) {
        rand_u32_index = 0;
    } else {
        rand_u32_index += 1;
    }
    return index;
}

u32 rand_u32() {
    u32 v = rand_u32_index_next();
    v = (v << 13) ^ v;
    return ((v * (v * v * 15731 + 789221) + 1376312589) & 0x7fffffff);
}
