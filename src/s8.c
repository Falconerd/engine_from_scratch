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

s8 s8_concat(s8 s1, s8 s2, allocator a) {
    s8 r = {0};
    u8 *mem = make(u8, s1.length + s2.length + 1, a);
    r.data = mem;
    mem_copy(mem, s1.data, s1.length);
    mem_copy(mem + s1.length, s2.data, s2.length);
    r.length = s1.length + s2.length;
    r.data[r.length] = 0;
    return r;
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

// Helper function to check for NaN
b32 isnan(f32 value) {
    return value != value;
}

// Helper function to check for infinity
b32 isinf(f32 value) {
    return !isnan(value) && isnan(value - value);
}

s8 s8_from_f32(f32 n, int precision, allocator a) {
    s8 r = {0};
    int i = 0;
    b32 is_negative = 0;
    r.data = make(u8, 24, a); // Buffer size may need adjustment based on precision
    if (!r.data) {
        __debugbreak();
        return r;
    }

    // Check for special values: NaN and infinities
    if (isnan(n)) {
        memcpy(r.data, "NaN", 3);
        r.length = 3;
        return r;
    }
    if (isinf(n)) {
        memcpy(r.data, (n < 0) ? "-inf" : "inf", (n < 0) ? 4 : 3);
        r.length = (n < 0) ? 4 : 3;
        return r;
    }

    if (n == 0.0f) {
        r.data[0] = '0';
        r.length = 1;
        return r;
    }

    if (n < 0) {
        is_negative = 1;
        n = -n;
    }

    // Separate integer and fractional parts
    i32 int_part = (i32)n;
    f32 frac_part = n - (f32)int_part;

    // Convert integer part
    i32 int_part_copy = int_part;
    int int_length = 0;
    while (int_part_copy) {
        int_part_copy /= 10;
        int_length++;
    }

    for (int j = int_length - 1; j >= 0; j--) {
        r.data[j] = (int_part % 10) + '0';
        int_part /= 10;
    }
    i += int_length;

    // Add decimal point
    r.data[i++] = '.';

    // Convert fractional part
    for (int p = 0; p < precision; p++) {
        frac_part *= 10.0f;
        int num = (int)(frac_part);
        r.data[i++] = (u8)('0' + num);
        frac_part -= (f32)num;
    }

    // Handle negative sign
    if (is_negative) {
        for (int j = i; j >= 0; j--) {
            r.data[j + 1] = r.data[j];
        }
        r.data[0] = '-';
        i++;
    }

    r.data[i] = '\0';
    r.length = i;
    return r;
}

#define s8_from_str(s, a) (s8_clone((s8){strlen(s), (u8 *)s}, a))

#endif

