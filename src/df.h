/*
    Dylan Falconer's Massive Header Library

    - Everything that needs to allocate should accept custom allocators.
    - Allocators will have a standard definition (perhaps look into nullprogram's new article).
    - The most useful things for me personally are prioritised.
    - Changes will be versioned.

    TODO:
        - [ ] Growable array (like vec).
        - [ ] Allocator definition.
*/

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "df_types.h"
#include "df_mem.h"
#include "df_allocator.h"
#include "df_arena.h"
#include "df_array.h"
#include "df_string.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
