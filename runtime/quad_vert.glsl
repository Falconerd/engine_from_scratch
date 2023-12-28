#version 140
in vec3 in_position;

out vec2 v_uvs;
out vec4 v_color;

uniform mat4 projection;
uniform mat4 model;

void main(void) {
    switch (gl_VertexID) {
        case 3:
        case 0: v_uvs = vec2(0.0, 0.0); v_color = vec4(1.0, 0.0, 0.0, 1.0); break;
        case 1: v_uvs = vec2(1.0, 0.0); v_color = vec4(0.0, 1.0, 0.0, 1.0); break;
        case 4:
        case 2: v_uvs = vec2(1.0, 1.0); v_color = vec4(0.0, 0.0, 1.0, 1.0); break;
        case 5: v_uvs = vec2(0.0, 1.0); v_color = vec4(1.0, 1.0, 0.0, 1.0); break;
    }
    gl_Position = projection * model * vec4(in_position, 1.0);
}
