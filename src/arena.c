#ifndef ARENA_INCLUDED
#define ARENA_INCLUDED

#define DEFAULT_ALIGNMENT (2 * sizeof(void *))

typedef struct {
    void *base;
    u64 size;
    u64 offset;
    u64 committed;
} arena;

#define arena_alloc_init(a) (allocator){arena_alloc, arena_free, a}

void *arena_alloc_aligned(arena *a, i64 size, i64 alignment) {
    uptr curr_ptr = (uptr)a->base + (uptr)a->offset;
    uptr offset = align_forward(curr_ptr, alignment);
    offset -= (uptr)a->base;

    if (offset + size > a->size) {
        return 0;
    }

    a->committed += size;
    void *ptr = (byte *)a->base + offset;
    a->offset = offset + size;

    return ptr;
}

void *arena_alloc(i64 size, void *context) {
    if (!size) {
        return 0;
    }

    return arena_alloc_aligned((arena *)context, size, DEFAULT_ALIGNMENT);
}

void arena_free(i64 size, void *ptr, void *context) {
    (void)ptr;
    (void)size;
    (void)context;
}

void arena_free_all(void *context) {
    arena *a = context;
    a->offset = 0;
    a->committed = 0;
}

#endif
