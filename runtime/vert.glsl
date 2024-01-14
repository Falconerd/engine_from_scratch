#version 140
in vec3 in_position;
// out vec3 ex_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void) {
    vec4 world_position = model * vec4(in_position, 1.0);
    gl_Position = projection * view * world_position;
    // ex_color = in_color;
}
