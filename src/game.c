#include "common.h"
#include "win32.h"

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

void game_reinit(game_memory *memory) {
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
}

void game_init(game_memory *memory) {

    game_reinit(memory);

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

    ////////////////////////////////////////////////////////////////////////////////////////////////

    gs->text_shader_id = draw_shader_create("runtime/text_vert.glsl", "runtime/text_frag.glsl", transient_allocator);
    assert(gs->text_shader_id);

    m4_ortho(&gs->projection, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.f, -1.f, 1.f);
    gs->model = m4_identity();
    m4_scale(&gs->model, v3(0.25f, 0.25f, 1.f));

    glGenVertexArrays(1, &gs->text_vao);
    glGenBuffers(1, &gs->text_vbo);
    glBindVertexArray(gs->text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, gs->text_vbo);
    // NOTE: Hardcoded value!
    glBufferData(GL_ARRAY_BUFFER, 2000 * sizeof(text_vertex), 0, GL_STATIC_DRAW);;
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

    os_file pie_file = os_file_read("runtime/crows-248838.pie", transient_allocator);
    byte *pie_buffer = make(byte, MB(10), transient_allocator);
    pie_decoded decoded = pie_decode(pie_file.data, pie_buffer, MB(10));
    u32 format = decoded.stride == 4 ? GL_RGBA : GL_RGB;

    ////////////////////////////////////////////////////////////////////////////////////////////////

    gs->quad_shader_id = draw_shader_create("runtime/quad_vert.glsl", "runtime/quad_frag.glsl", transient_allocator);
    assert(gs->quad_shader_id);

    m4_ortho(&gs->quad_projection, 0.f, WINDOW_WIDTH, 0.f, WINDOW_HEIGHT, -1.f, 1.f);
    gs->quad_model = m4_identity();
    m4_translate(&gs->quad_model, v3(0.0f, 0.0f, 0.f));
    m4_scale(&gs->quad_model, v3(300.f, 300.f, 1.f));

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, decoded.width, decoded.height, 0,
                format, GL_UNSIGNED_BYTE, pie_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void game_update_and_render(game_memory *memory, input_state *input, i32 load_timer) {
    (void)input;

    if (!memory->is_initialized) {
        game_reinit(memory);
    }

    glUseProgram(gs->shader_id);
    {
        glBindVertexArray(gs->vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
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
        glBindBuffer(GL_ARRAY_BUFFER, gs->text_vbo);
        
        s8 lt_text = s8_from_i32(load_timer, transient_allocator);
        result text_verts = text_write(v3(10.f, 80.f, 0.f), v2(0.25f, 0.25f), monofonto_atlas_data, lt_text, transient_allocator);
        // NOTE: Apparently it's faster to destroy and create a new buffer.
        // We should test that.
        glBufferSubData(GL_ARRAY_BUFFER, 0, text_verts.size * sizeof(text_vertex), text_verts.data);
        glDrawArrays(GL_TRIANGLES, 0, lt_text.length * 6);
    }

    gs->frame += 1;
    arena_free_all(&transient_arena);
}

#ifdef _WIN32

// TODO: I'm not sure, but loading these twice is bleh.
void *win32_gl_proc_load(char *name) {
    void *p = (void *)wglGetProcAddress(name);
    if(p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1)) {
        void *module = LoadLibrary("opengl32.dll");
        p = (void *)GetProcAddress(module, name);
    }
    return p;
}

#define win32_gl_proc_load_assign(n) n = (n##def *)win32_gl_proc_load(#n); assert(n && "Failed to load n");

void game_load_gl_functions(void) {
    win32_gl_proc_load_assign(glCreateShader);
    win32_gl_proc_load_assign(glCompileShader);
    win32_gl_proc_load_assign(glShaderSource);
    win32_gl_proc_load_assign(glGetShaderiv);
    win32_gl_proc_load_assign(glGetShaderInfoLog);
    win32_gl_proc_load_assign(glCreateProgram);
    win32_gl_proc_load_assign(glAttachShader);
    win32_gl_proc_load_assign(glLinkProgram);
    win32_gl_proc_load_assign(glGetProgramInfoLog);
    win32_gl_proc_load_assign(glGetProgramiv);
    win32_gl_proc_load_assign(glGenVertexArrays);
    win32_gl_proc_load_assign(glBindVertexArray);
    win32_gl_proc_load_assign(glGenBuffers);
    win32_gl_proc_load_assign(glBindBuffer);
    win32_gl_proc_load_assign(glBufferData);
    win32_gl_proc_load_assign(glBufferSubData);
    win32_gl_proc_load_assign(glVertexAttribPointer);
    win32_gl_proc_load_assign(glEnableVertexAttribArray);
    win32_gl_proc_load_assign(glBindVertexArray);
    win32_gl_proc_load_assign(glDrawArrays);
    win32_gl_proc_load_assign(glGetError);
    win32_gl_proc_load_assign(glUseProgram);
    win32_gl_proc_load_assign(glGenTextures);
    win32_gl_proc_load_assign(glBindTexture);
    win32_gl_proc_load_assign(glTexImage2D);
    win32_gl_proc_load_assign(glTexParameteri);
    win32_gl_proc_load_assign(glActiveTexture);
    win32_gl_proc_load_assign(glGetUniformLocation);
    win32_gl_proc_load_assign(glUniform1i);
    win32_gl_proc_load_assign(glUniformMatrix4fv);
    win32_gl_proc_load_assign(glViewport);
    win32_gl_proc_load_assign(glEnable);
    win32_gl_proc_load_assign(glBlendFunc);
}

int _DllMainCRTStartup(void *inst, int reason, void *res) {
    (void)inst; (void)reason; (void)res;
    return 1;
}
#endif
