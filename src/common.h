#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: Investigate what this really means for floating point support.
extern int _fltused = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////

typedef __int32             b32;
typedef unsigned __int8     u8;
typedef unsigned __int16    u16;
typedef unsigned __int32    u32;
typedef unsigned __int64    u64;
typedef __int32             i32;
typedef __int64             i64;
typedef float               f32;
typedef double              f64;
typedef char                byte;
typedef __int64             size;
typedef unsigned __int64    uptr;

////////////////////////////////////////////////////////////////////////////////////////////////////

#define assert(x) if (!(x)) { __debugbreak(); }

////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_U8  0x00000000000000ff
#define MAX_U16 0x000000000000ffff
#define MAX_U32 0x00000000ffffffff
#define MAX_U64 0xffffffffffffffff

////////////////////////////////////////////////////////////////////////////////////////////////////

#define KB(x) (x * 1024ULL)
#define MB(x) (KB(x) * 1024ULL)
#define GB(x) (MB(x) * 1024ULL)
#define TB(x) (GB(x) * 1024ULL)

////////////////////////////////////////////////////////////////////////////////////////////////////

#define WINDOW_TITLE "Project: MP"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    void *(*alloc)(i64, void *context);
    void (*free)(i64, void *, void *context);
    void *context;
} allocator;

////////////////////////////////////////////////////////////////////////////////////////////////////

#define make(t, n, a) (t *)((a).alloc(sizeof(t) * n, (a).context))
#define delete(s, p, a) ((a).free(s, p, (a).context))

////////////////////////////////////////////////////////////////////////////////////////////////////

#define is_power_of_two(x) ((x != 0) && ((x & (x - 1)) == 0))

////////////////////////////////////////////////////////////////////////////////////////////////////

// If the size is > 0, then we can assume all is OK.
// EG: File read result: size = bytes read. Data is the bytes.
// EG: Perform some operation that may fail, size = 1 (success), data is the passed in argument.
typedef struct {
    u64 size;
    void *data;
} result;

////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    b32 is_initialized;
    u64 permanent_storage_size;
    void *permanent_storage;
    u64 transient_storage_size;
    void *transient_storage;
} game_memory;

////////////////////////////////////////////////////////////////////////////////////////////////////

// Just add `def` to the end for the type.
// Declare Type-Definition+Function-Pointer macro.
#define TDFP_INNER(a, b) a##def##b
#define TDFP(return_type, name, params) typedef return_type TDFP_INNER(name, params); TDFP_INNER(name, *) name;
