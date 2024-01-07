#ifndef ARRAY_INCLUDED
#define ARRAY_INCLUDED

/*
    This is a dynamic array library.
    It requires a custom allocator and does not guarantee pointer safety.
    It may reallocate when items are pushed into it.

    Example usage:
        allocator a = ...;
        i64 capacity = 8;
        int *arr = array(int, capacity, a);

        array_append(x, 42);
        array_append(x, 23);
        array_append(x, 13);
        array_append(x, 17);

        assert(arr[0] == 42);
        assert(arr[1] == 23);
        assert(arr[2] == 13);
        assert(arr[3] == 17);

    When array is invoked, size for the capacity plus a header is allocated using the provided
    allocator.

    The header has this memory layout:
        bits | description
        ------------------
          64 | capacity (signed)
          64 | length (signed)
          64 | alloc function pointer
          64 | free function pointer
          64 | allocator context pointer
    Directly after the header is the memory for the items themselves.
        | header   | items                    | remaining capacity
        | 40 bytes | item_size * length bytes | 

    We don't have to keep the item size because operations will include a typed parameter that we
    can use sizeof on.
*/

#include "common.h"

// For convenience, we can just keep a header struct type here.
// It's also nice to be able to cast in the debugger.
typedef struct {
    i64 capacity;
    i64 length;
    allocator a;
} array_h;

#define ARRAY_INITIAL_CAPACITY (16)

#define array(t, a) (t *)array_init(sizeof(t), ARRAY_INITIAL_CAPACITY, a)

void *array_init(i64 item_size, i64 capacity, allocator a) {
    byte *mem = make(byte, item_size * capacity + sizeof(array_h), a);
    if (!mem) {
        return 0;
    }

    array_h *h = (array_h *)mem;
    h->capacity = capacity;
    h->length = 0;
    h->a = a;

    return (mem + sizeof(array_h));
}

#define array_header(a) ((array_h *)(a) - 1)

void *array_ensure_capacity(void *a, i64 item_count, i64 item_size) {
    array_h *h = array_header(a);
    i64 desired_capacity = h->length + item_count;
    if (h->capacity < desired_capacity) {

        // Allocate enough space.
        i64 new_capacity = h->capacity * 2;
        while (new_capacity < desired_capacity) {
            new_capacity *= 2;
            if (new_capacity < h->capacity) {
                return 0;
            }
        }

        i64 old_length = sizeof(array_h) + h->length * item_size;
        i64 size = sizeof(array_h) + new_capacity * item_size;
        void *new_mem = h->a.alloc(size, h->a.context);
        if (!new_mem) {
            return 0;
        }

        // Copy data to new allocation.
        mem_copy(new_mem, h, old_length);

        // Naively try to free the old array.
        // Only free if a free function is available.
        // Sometimes memory will be managed externally - e.g. scratch buffers.
        if (h->a.free) {
            i64 old_size = sizeof(array_h) + h->length * item_size;
            delete(old_size, h, h->a);
        }

        h = (array_h *)new_mem;

        // Update data.
        h->capacity = new_capacity;
    }

    return h + 1;
}

#define array_length(a) (((array_h *)a - 1)->length)

#define array_append(a, v) ( \
    (a) = array_ensure_capacity(a, 1, sizeof(v)), \
    (a)[array_header(a)->length++] = (v))

#define array_remove(a, i) do { \
    array_h *h = array_header(a); \
    if (h->length == 0) { \
    } else if (i == h->length - 1) { \
        h->length -= 1; \
    } else if (h->length > 1) { \
        void *ptr = &a[i]; \
        void *last = &a[h->length - 1]; \
        h->length -= 1; \
        mem_copy(ptr, last, sizeof(*a)); \
    } \
} while (0);

#endif

