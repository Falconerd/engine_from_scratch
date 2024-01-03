#ifndef S8_INCLUDED
#define S8_INCLUDED

typedef struct {
    i64 length;
    u8 *data;
} s8;

#define s8(s) (s8){strlen(s), (u8 *)s}

s8 s8_clone(s8 s, allocator a) {
    s8 r = {0};
    r.data = make(u8, s.length + 1, a);
    if (!r.data) {
        return r;
    }
    mem_copy(r.data, s.data, s.length);
    r.data[s.length] = 0;
    r.length = s.length;
    return r;
}

b32 s8_contains(s8 haystack, s8 needle) {
    u8 *curr = haystack.data;
    u8 *match_char = needle.data;
    i64 match_length = 0;
    while (*curr) {
        if (*curr == *match_char) {
            match_char += 1;
            match_length += 1;
        } else {
            match_length = 0;
            match_char = needle.data;
        }
        curr += 1;

        if (match_length == needle.length) {
            return 1;
        }
    }
    return 0;
}

#define s8_from_str(s, a) (s8_clone((s8){strlen(s), (u8 *)s}, a))

#endif

