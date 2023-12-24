#ifndef DF_STRING
#define DF_STRING

#include "df_mem.h"

#ifndef DF_STRING_ENABLE_PREFIX
#define s8 df_s8
#ifndef DF_STRING_NO_STRLEN
#define strlen df_strlen
#endif
#define s8clone df_s8clone
#define s8fromstr df_s8fromstr
#endif

typedef struct {
    i64 length;
    u8 *data;
} df_s8;

#define df_s8(s) (df_s8){df_strlen(s), s}

i64 df_strlen(const char *s) {
    i64 r = 0;
    while (*s++) r += 1;
    return r;
}

df_s8 df_s8clone(df_s8 s, df_allocator a) {
    df_s8 r = {0};
    r.data = df_make(u8, s.length + 1, a);
    if (!r.data) {
        return r;
    }
    df_memcopy(r.data, s.data, s.length);
    r.data[s.length] = 0;
    r.length = s.length;
    return r;
}

#define df_s8fromstr(s, a) (df_s8clone((df_s8){df_strlen(s), (u8 *)s}, a))

#endif
