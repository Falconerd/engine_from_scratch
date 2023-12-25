typedef struct {
    i64 length;
    u8 *data;
} s8;

#define s8(s) (s8){strlen(s), (u8 *)s}

s8 s8clone(s8 s, allocator a) {
    s8 r = {0};
    r.data = make(u8, s.length + 1, a);
    if (!r.data) {
        return r;
    }
    memcopy(r.data, s.data, s.length);
    r.data[s.length] = 0;
    r.length = s.length;
    return r;
}

b32 s8contains(s8 haystack, s8 needle) {
    u8 *curr = haystack.data;
    i64 match_length = 0;
    while (*curr) {
        if (*curr)
    }
}

#define s8fromstr(s, a) (s8clone((s8){strlen(s), (u8 *)s}, a))
