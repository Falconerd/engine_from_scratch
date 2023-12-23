#ifndef DF_STRING
#define DF_STRING

#include "df_mem.h"

typedef struct {
    i64 length;
    u8 *data;
} s8;

#define s8(s) (s8){df_strlen(s), s}

i64 df_strlen(const char *s) {
    i64 r = 0;
    while (*s++) r += 1;
    return r;
}

s8 s8_clone(s8 s, allocator a) {
    s8 r = {0};
    r.data = df_make(u8, s.length + 1, a);
    if (!r.data) {
        return r;
    }
    df_mem_copy(r.data, s.data, s.length);
    r.data[s.length] = 0;
    r.length = s.length;
    return r;
}

#define s8_from_str(s, a) (s8_clone((s8){df_strlen(s), (u8 *)s}, a))

#endif
