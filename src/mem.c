#define make(t, n, a) (t *)((a).alloc(sizeof(t) * n, (a).context))
#define delete(s, p, a) ((a).free(s, p, (a).context))
#define is_power_of_two(x) ((x != 0) && ((x & (x - 1)) == 0))

void memcopy(void *dest, void *src, i64 size) {
    u8 *d = (u8 *)dest;
    u8 *s = (u8 *)src;
    for (i64 i = 0; i < size; i += 1) {
        d[i] = s[i];
    }
}

uptr align_forward(uptr ptr, i64 alignment) {
    uptr p, a, modulo;
    if (!is_power_of_two(alignment)) {
        return 0;
    }

    p = ptr;
    a = (uptr)alignment;

    modulo = p & (a - 1);

    if (modulo) {
        p += a - modulo;
    }

    return p;
}

b32 is_aligned(void *ptr, i64 alignment) {
    return ((uptr)ptr % alignment) == 0;
}