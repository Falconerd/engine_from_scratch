#ifndef DF_ALLOCATOR
#define DF_ALLOCATOR

// All my libraries use this allocator interface.

typedef struct {
    void *(*alloc)(i64, void *context);
    void (*free)(i64, void *, void *context);
    void *context;
} allocator;

#endif
