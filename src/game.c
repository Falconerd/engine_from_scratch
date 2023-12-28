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
    u32 arial_texture;
    m4 projection;
    m4 model;
    // 2D textured, colored quad rendering.
    m4 quad_projection;
    m4 quad_model;
    u32 quad_shader_id;
    u32 quad_vao;
    u32 quad_vbo;
    u32 test_texture;
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

    ////////////////////////////////////////////////////////////////////////////////////////////////

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    ////////////////////////////////////////////////////////////////////////////////////////////////

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

    ////////////////////////////////////////////////////////////////////////////////////////////////

    os_file font_tga_file = os_file_read("runtime/monofonto.tga", transient_allocator);
    size font_texture_width, font_texture_height;
    tga_dimensions(font_tga_file.data, &font_texture_width, &font_texture_height);
    result font_rgb_bytes = rgb_from_tga(font_tga_file.data, transient_allocator);
    assert(font_rgb_bytes.size && "Failed to create font data.");

    result text_verts = text_write(v3(10.f, 10.f, 0.f), monofonto_atlas_data, s8("Player Pos: 1.34, 32.23, 0.78"), transient_allocator);

    ////////////////////////////////////////////////////////////////////////////////////////////////

    gs->text_shader_id = draw_shader_create("runtime/text_vert.glsl", "runtime/text_frag.glsl", transient_allocator);
    assert(gs->text_shader_id);

    m4_ortho(&gs->projection, 0.f, WINDOW_WIDTH, 0.f, WINDOW_HEIGHT, -1.f, 1.f);
    gs->model = m4_identity();
    m4_scale(&gs->model, v3(0.25f, 0.25f, 1.f));

    glGenVertexArrays(1, &gs->text_vao);
    glGenBuffers(1, &gs->text_vbo);
    glBindVertexArray(gs->text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, gs->text_vbo);
    glBufferData(GL_ARRAY_BUFFER, text_verts.size * sizeof(text_vertex), text_verts.data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(text_vertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, sizeof(text_vertex), (void *)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, 0, sizeof(text_vertex), (void *)(5 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &gs->arial_texture);
    glBindTexture(GL_TEXTURE_2D, gs->arial_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, font_texture_width, font_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, font_rgb_bytes.data);
    glBindTexture(GL_TEXTURE_2D, 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////

    os_file pie_file = os_file_read("runtime/test_embedded_palette_v2.pie", transient_allocator);
    // Non-allocating - uses supplied data.
    pie_image pie_image = pie_from_bytes(pie_file.data);
    result pie_rgb = pie_rgb_from_pie(pie_image, transient_allocator);

    (void)pie_image;
    (void)pie_file;
    (void)pie_rgb;

    ////////////////////////////////////////////////////////////////////////////////////////////////

    gs->quad_shader_id = draw_shader_create("runtime/quad_vert.glsl", "runtime/quad_frag.glsl", transient_allocator);
    assert(gs->quad_shader_id);

    m4_ortho(&gs->quad_projection, 0.f, WINDOW_WIDTH, 0.f, WINDOW_HEIGHT, -1.f, 1.f);
    gs->quad_model = m4_identity();
    m4_scale(&gs->quad_model, v3(25.f, 25.f, 1.f));

    f32 quad_vertices[] = {
        0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 1.f, 0.f,
        0.f, 0.f, 0.f,
        1.f, 1.f, 0.f,
        0.f, 1.f, 0.f
    };

    glGenVertexArrays(1, &gs->quad_vao);
    glGenBuffers(1, &gs->quad_vbo);
    glBindVertexArray(gs->quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, gs->quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(f32), (void *)0);
    glEnableVertexAttribArray(0);

    glGenTextures(1, &gs->test_texture);
    glBindTexture(GL_TEXTURE_2D, gs->test_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, font_texture_width, font_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pie_rgb.data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void game_update_and_render(input_state *input) {
    if (input->forward.down) {
        MessageBox(0, "ARST", "ARST", 0);
        input->forward.down = 0;
    }

    glUseProgram(gs->shader_id);
    {
        glBindVertexArray(gs->vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glUseProgram(gs->text_shader_id);
    {
        i32 uloc = glGetUniformLocation(gs->text_shader_id, "projection");
        glUniformMatrix4fv(uloc, 1, 0, &gs->projection.data[0][0]);
        uloc = glGetUniformLocation(gs->text_shader_id, "model");
        glUniformMatrix4fv(uloc, 1, 0, &gs->model.data[0][0]);
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, gs->arial_texture);
        uloc = glGetUniformLocation(gs->text_shader_id, "msdf");
        glUniform1i(uloc, 0);
        glUseProgram(gs->text_shader_id);
        glBindVertexArray(gs->text_vao);
        glDrawArrays(GL_TRIANGLES, 0, 132 * 6);
    }

    glUseProgram(gs->quad_shader_id);
    {
        i32 uloc = glGetUniformLocation(gs->quad_shader_id, "projection");
        glUniformMatrix4fv(uloc, 1, 0, &gs->quad_projection.data[0][0]);
        uloc = glGetUniformLocation(gs->quad_shader_id, "model");
        glUniformMatrix4fv(uloc, 1, 0, &gs->quad_model.data[0][0]);
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, gs->test_texture);
        glBindVertexArray(gs->quad_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    gs->frame += 1;
}
