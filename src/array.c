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

        array_push(x, 42);
        array_push(x, 23);
        array_push(x, 13);
        array_push(x, 17);

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

#define array(t, a) (t *)array_fn(sizeof(t), 8, a)

void *array_fn(i64 item_size, i64 capacity, allocator a) {
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

void *array_ensure_capacity_fn(void *a, i64 item_count, i64 item_size) {
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

#define array_put(a, v) ( \
    (a) = array_ensure_capacity_fn(a, 1, sizeof(v)), \
    (a)[array_header(a)->length++] = (v))
#define array_append array_put

void array_put_fn(array_h *h, i64 item_size, void *item) {
    if (h->capacity == h->length) {
        // TODO: resize
        // h = array_resize(h) as memory location may change.
    }

    byte *dest = (byte *)h;
    dest += sizeof(array_h) + item_size * h->length;
    mem_copy(dest, item, item_size);
}

#endif

