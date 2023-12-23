#ifndef DF_MEM
#define DF_MEM

void df_mem_copy(void *dest, void *src, s64 size) {
    u8 *d = (u8 *)dest;
    u8 *s = (u8 *)src;
    for (s64 i = 0; i < size; i += 1) {
        d[i] = s[i];
    }
}

#endif
