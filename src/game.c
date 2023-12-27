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
    u32 shader_id;
    u32 vao;
    u32 vbo;
    u32 text_shader_id;
    u32 text_vao;
    u32 text_vbo;
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

    // NOTE: Move these somewhere appropriate.
    gs->shader_id = draw_shader_create("runtime/vert.glsl", "runtime/frag.glsl", transient_allocator);
    assert(gs->shader_id && "Failed to create shader.");

    f32 vertices[] = {
        -0.5f, -0.5f, 0.f,
         0.5f, -0.5f, 0.f,
         0.0f,  0.5f, 0.f,
    };

    f32 colors[] = {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
    };
    (void)colors;

    glGenVertexArrays(1, &gs->vao);
    glGenBuffers(1, &gs->vbo);
    glBindVertexArray(gs->vao);
    glBindBuffer(GL_ARRAY_BUFFER, gs->vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(f32), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(f32), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    gs->text_shader_id = draw_shader_create("runtime/text_vert.glsl", "runtime/text_frag.glsl", transient_allocator);

    #define top_left    -0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f
    #define top_right    0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f
    #define bottom_right 0.5f,  0.5f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f
    #define bottom_left -0.5f,  0.5f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f
    f32 quad_vertices[] = {
        bottom_left, bottom_right, top_right,
        bottom_left, top_right, top_left,
    };

    glGenVertexArrays(1, &gs->text_vao);
    glGenBuffers(1, &gs->text_vbo);
    glBindVertexArray(gs->text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, gs->text_vbo);
    glBufferData(GL_ARRAY_BUFFER, 60 * sizeof(f32), quad_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 10 * sizeof(f32), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, 0, 10 * sizeof(f32), (void *)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, 0, 10 * sizeof(f32), (void *)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &gs->arial_texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 16, 16, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, msdf_image_data);
// TDFP(void, glTexImage2D, (i32 target, i32 level, i32 internalformat, size width, size height, i32 border, u32 format, u32 type, void *data));
}

void game_update_and_render(input_state *input) {
    if (input->forward.down) {
        MessageBox(0, "ARST", "ARST", 0);
        input->forward.down = 0;
    }

    // glUseProgram(gs->shader_id);
    // glBindVertexArray(gs->vao);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(gs->text_shader_id);
    glBindVertexArray(gs->text_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    gs->frame += 1;
}
