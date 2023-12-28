#version 140
in vec3 in_position;
out vec2 v_uvs;

uniform mat4 projection;
uniform mat4 model;

void main(void) {
    switch (gl_VertexID) {
        case 0: v_uvs = vec2(0.0, 0.0);
        case 1: v_uvs = vec2(1.0, 0.0);
        case 2: v_uvs = vec2(1.0, 1.0);
        case 3: v_uvs = vec2(0.0, 1.0);
    }
    gl_Position = projection * model * vec4(in_position, 1.0);
}
