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

s8 s8_from_i32(i32 n, allocator a) {
    s8 r = {0};
    int i = 0;
    b32 is_negative = 0;
    r.data = make(u8, 11, a);
    if (!r.data) {
        __debugbreak();
        return r;
    }

    if (!n) {
        r.data[0] = '0';
        r.length = 1;
        return r;
    }

    if (n < 0) {
        is_negative = 1;
        n = -n;
    }

    while (n) {
        u8 rem = n % 10;
        r.data[i] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        n = n / 10;
        i += 1;
    }

    if (is_negative) {
        r.data[i] = '-';
        i += 1;
    }

    r.data[i] = 0;

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = r.data[start];
        r.data[start] = r.data[end];
        r.data[end] = temp;
        start += 1;
        end -= 1;
    }
    
    r.length = i;
    return r;
}

#define s8_from_str(s, a) (s8_clone((s8){strlen(s), (u8 *)s}, a))

#endif

