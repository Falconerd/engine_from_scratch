#define make(t, n, a) (t *)((a).alloc(sizeof(t) * n, (a).context))
#define delete(s, p, a) ((a).free(s, p, (a).context))

void memcopy(void *dest, void *src, i64 size) {
    u8 *d = (u8 *)dest;
    u8 *s = (u8 *)src;
    for (i64 i = 0; i < size; i += 1) {
        d[i] = s[i];
    }
}
