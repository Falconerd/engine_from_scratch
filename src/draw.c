#include "gl.h"

u32 draw_shader_create(char *vp, char *fp, allocator a) {
    os_file vf = os_file_read(vp, a);
    if (!vf.size) return 0;
    os_file ff = os_file_read(fp, a);
    if (!ff.size) return 0;

    u32 id = 0;
    b32 ok = 0;
    char log[512] = {0};

    u32 shader_v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_v, 1, (char **)&vf.data, 0);
    glCompileShader(shader_v);
    glGetShaderiv(shader_v, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader_v, 512, 0, log);
        return 0;
    }

    u32 shader_f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_f, 1, (char **)&ff.data, 0);
    glCompileShader(shader_f);
    glGetShaderiv(shader_f, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader_f, 512, 0, log);
        return 0;
    }

    id = glCreateProgram();
    glAttachShader(id, shader_v);
    glAttachShader(id, shader_f);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(id, 512, 0, log);
        return 0;
    }

    if (!id) {
        return 0;
    }

    return id;
}
