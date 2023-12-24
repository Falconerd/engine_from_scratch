#ifndef DF_MEM
#define DF_MEM

#ifndef DF_MEM_ENABLE_PREFIX
#define memcopy df_memcopy
#ifndef DF_MEM_NO_MAKE
#define make df_make
#endif
#ifndef DF_MEM_NO_DELELE
#define delete df_delete
#endif
#endif

#define df_make(t, n, a) (t *)((a).alloc(sizeof(t) * n, (a).context))
#define df_delete(s, p, a) ((a).free(s, p, (a).context))

void df_memcopy(void *dest, void *src, i64 size) {
    u8 *d = (u8 *)dest;
    u8 *s = (u8 *)src;
    for (i64 i = 0; i < size; i += 1) {
        d[i] = s[i];
    }
}

#endif
