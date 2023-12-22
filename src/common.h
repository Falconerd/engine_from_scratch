#ifndef COMMON_H
#define COMMON_H

typedef __int32             b32;
typedef unsigned __int8     u8;
typedef unsigned __int16    u16;
typedef unsigned __int32    u32;
typedef unsigned __int64    u64;
typedef __int8              i8;
typedef __int16             i16;
typedef __int32             i32;
typedef __int64             i64;

///////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_U8  0x00000000000000ff
#define MAX_U16 0x000000000000ffff
#define MAX_U32 0x00000000ffffffff
#define MAX_U64 0xffffffffffffffff

///////////////////////////////////////////////////////////////////////////////////////////////////

#define KB(x) (x * 1024ULL)
#define MB(x) (KB(x) * 1024ULL)
#define GB(x) (MB(x) * 1024ULL)
#define TB(x) (GB(x) * 1024ULL)
#define ASSERT(x) if (!(x)) { __debugbreak(); }

///////////////////////////////////////////////////////////////////////////////////////////////////

#define WINDOW_TITLE "Test"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BUILD_EXTERNAL

typedef struct {
    u32 size;
    void *data;
} read_file_result;

read_file_result debug_platform_read_entire_file(const char *path);
void debug_platform_free_file_memory(void *ptr);
b32 debug_platform_write_entire_file(const char *path, u64 size, void *ptr);

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    b32 is_initialized;
    u64 permanent_storage_size;
    void *permanent_storage;
    u64 transient_storage_size;
    void *transient_storage;
} game_memory;

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif

