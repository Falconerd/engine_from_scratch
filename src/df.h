/*
    Dylan Falconer's Massive Header Library

    - Everything that needs to allocate should accept custom allocators.
    - Allocators will have a standard definition (perhaps look into nullprogram's new article).
    - The most useful things for me personally are prioritised.
    - Changes will be versioned.
    - Everything is Win32 only.

    TODO:
        - [x] Growable array (like vec).
        - [x] Allocator definition.
        - [x] Load Files.
*/

#ifndef DF_NO_ASSERT
#define assert df_assert
#endif
#define df_assert(x) if (!(x)) { __debugbreak(); }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KB(x) (x * 1024ULL)
#define MB(x) (KB(x) * 1024ULL)
#define GB(x) (MB(x) * 1024ULL)
#define TB(x) (GB(x) * 1024ULL)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "df_types.h"
#include "df_limits.h"
#include "df_win32.h"
#include "df_mem.h"
#include "df_allocator.h"
#include "df_arena.h"
#include "df_array.h"
#include "df_string.h"
#include "df_os.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
