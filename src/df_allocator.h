#ifndef DF_ALLOCATOR
#define DF_ALLOCATOR

// All my libraries use this allocator interface.

typedef struct {
    void *(*alloc)(s64, void *context);
    void (*free)(s64, void *, void *context);
    void *context;
} allocator;

#endif
