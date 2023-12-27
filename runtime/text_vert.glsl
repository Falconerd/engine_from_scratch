#version 140
in vec3 in_pos;
in vec3 in_uvs;
in vec4 in_color;

out vec3 v_uvs;
out vec4 v_color;

uniform mat4 mvp;

void main() {
    gl_Position = vec4(in_pos, 1.0);
    // gl_Position = mvp * vec4(in_pos, 1.0);
    v_uvs = in_uvs;
    v_color = in_color;
}
