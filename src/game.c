#include "common.h"
#include "win32.h"

typedef struct {
    int half_transitions;
    b32 down;
} key_state;

typedef struct {
    key_state forward;
    key_state backward;
    key_state left;
    key_state right;
} input_state;

typedef struct game_state {
    b32 is_running;
    u64 frame;
} game_state;

game_state *gs = 0;
arena permanent_arena;
arena transient_arena;
allocator permanent_allocator;
allocator transient_allocator;

void game_init(game_memory *memory) {
    permanent_arena = (arena){
        .base = memory->permanent_storage,
        .size = memory->permanent_storage_size,
    };
    transient_arena = (arena){
        .base = memory->transient_storage,
        .size = memory->transient_storage_size,
    };
    permanent_allocator = arena_alloc_init(&permanent_arena);
    transient_allocator = arena_alloc_init(&transient_arena);
    gs = make(game_state, 1, permanent_allocator);

    u32 sid = draw_shader_create("data/vert.glsl", "data/frag.glsl", transient_allocator);
    (void)sid;
}

void game_update_and_render(input_state *input) {
    if (input->forward.down) {
        MessageBox(0, "ARST", "ARST", 0);
        input->forward.down = 0;
    }

    // draw_triangle();

    gs->frame += 1;
}
