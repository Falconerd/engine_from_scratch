#version 140
in vec3 in_pos;
in vec2 in_uvs;
in vec4 in_color;

out vec2 v_uvs;
out vec4 v_color;

uniform mat4 projection;
uniform mat4 model;

void main() {
    // gl_Position = vec4(in_pos.x - 0.5, in_pos.y - 0.5, in_pos.z, 1.0);
    gl_Position = projection * model * vec4(in_pos.xy, 0.0, 1.0);
    v_uvs = in_uvs;
    v_color = in_color;
}
