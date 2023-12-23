/*
    This is a dynamic array library.
    It requires a custom allocator and does not guarantee pointer safety.
    It may reallocate when items are pushed into it.

    Example usage:
        allocator a = ...;
        s64 capacity = 8;
        int *arr = df_array(int, capacity, a);

        df_array_push(x, 42);
        df_array_push(x, 23);
        df_array_push(x, 13);
        df_array_push(x, 17);

        assert(arr[0] == 42);
        assert(arr[1] == 23);
        assert(arr[2] == 13);
        assert(arr[3] == 17);

    When df_array is invoked, size for the capacity plus a header is allocated
    using the provided allocator.

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

    We don't have to keep the item size because operations will include a typed
    parameter that we can use sizeof on.
*/
#ifndef DF_ARRAY
#define DF_ARRAY

#include "df_types.h"
#include "df_allocator.h"
#include "df_mem.h"

// For convenience, we can just keep a header struct type here.
// It's also nice to be able to cast in the debugger.
typedef struct {
    s64 capacity;
    s64 length;
    allocator a;
} df_array_h;

#define df_array(t, a) (t *)df_array_fn(sizeof(t), 8, a)

void *df_array_fn(s64 item_size, s64 capacity, allocator a) {
    u8 *mem = a.alloc(item_size * capacity + sizeof(df_array_h), a.context);
    if (!mem) {
        return 0;
    }

    df_array_h *h = (df_array_h *)mem;
    h->capacity = capacity;
    h->length = 0;
    h->a = a;

    return (mem + sizeof(df_array_h));
}

#define df_array_header(a) ((df_array_h *)(a) - 1)

void *df_array_ensure_capacity_fn(void *a, s64 item_count, s64 item_size) {
    df_array_h *h = df_array_header(a);
    s64 desired_capacity = h->length + item_count;
    if (h->capacity < desired_capacity) {

        // Allocate enough space.
        s64 new_capacity = h->capacity * 2;
        while (new_capacity < desired_capacity) {
            new_capacity *= 2;
            if (new_capacity < h->capacity) {
                return 0;
            }
        }

        s64 old_length = sizeof(df_array_h) + h->length * item_size;
        s64 size = sizeof(df_array_h) + new_capacity * item_size;
        void *new_mem = h->a.alloc(size, h->a.context);
        if (!new_mem) {
            return 0;
        }

        // Copy data to new allocation.
        df_mem_copy(new_mem, h, old_length);

        // Naively try to free the old array.
        // Only free if a free function is available.
        // Sometimes memory will be managed externally - e.g. scratch buffers.
        if (h->a.free) {
            s64 old_size = sizeof(df_array_h) + h->length * item_size;
            h->a.free(old_size, h, h->a.context);
        }

        h = (df_array_h *)new_mem;

        // Update data.
        h->capacity = new_capacity;
    }

    return h + 1;
}

#define df_array_put(a, v) ( \
    (a) = df_array_ensure_capacity_fn(a, 1, sizeof(v)), \
    (a)[df_array_header(a)->length++] = (v))

void df_array_put_fn(df_array_h *h, s64 item_size, void *item) {
    if (h->capacity == h->length) {
        // TODO: resize
        // h = df_array_resize(h) as memory location may change.
    }

    u8 *dest = (u8 *)h;
    dest += sizeof(df_array_h) + item_size * h->length;
    df_mem_copy(dest, item, item_size);
}

#endif
