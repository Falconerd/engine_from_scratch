#include "common.h"

inline u32 safe_truncate_u64(u64 v) {
    ASSERT(v <= MAX_U32);
    u32 r = (u32)v;
    return r;
}

