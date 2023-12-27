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
    u32 vao;
    u32 vbo;
    u32 glerr;
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

    u32 shader_id = draw_shader_create("data/vert.glsl", "data/frag.glsl", transient_allocator);
    assert(shader_id && "Failed to create shader.");

    f32 vertices[] = {
        -0.3f, -0.5f, -1.f,
        -0.8f, -0.5f, -1.f,
         0.2f, -0.5f, -1.f,
    };

    f32 colors[] = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
    };
    (void)colors;

    glGenVertexArrays(1, &gs->vao);
    gs->glerr = glGetError();
    assert(gs->glerr == 0);
    glGenBuffers(1, &gs->vbo);
    gs->glerr = glGetError();
    assert(gs->glerr == 0);
    glBindBuffer(GL_ARRAY_BUFFER, gs->vbo);
    gs->glerr = glGetError();
    assert(gs->glerr == 0);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(f32), vertices, GL_STATIC_DRAW);
    gs->glerr = glGetError();
    assert(gs->glerr == 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    gs->glerr = glGetError();
    assert(gs->glerr == 0);
    glEnableVertexAttribArray(0);
    gs->glerr = glGetError();
    assert(gs->glerr == 0);

    __debugbreak();
}

void game_update_and_render(input_state *input) {
    if (input->forward.down) {
        MessageBox(0, "ARST", "ARST", 0);
        input->forward.down = 0;
    }

    // draw_triangle();
    glBindVertexArray(gs->vao);
    gs->glerr = glGetError();
    assert(gs->glerr == 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    gs->glerr = glGetError();
    assert(gs->glerr == 0);

    gs->frame += 1;
}
