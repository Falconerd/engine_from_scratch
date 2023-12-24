#ifndef DF_ALLOCATOR
#define DF_ALLOCATOR

// All my libraries use this allocator interface.
#ifndef DF_ALLOCATOR_ENABLE_PREFIX
#define allocator df_allocator
#endif

typedef struct {
    void *(*alloc)(i64, void *context);
    void (*free)(i64, void *, void *context);
    void *context;
} df_allocator;

#endif
