#version 140
in vec3 in_position;
in vec3 in_color;
out vec3 ex_color;

void main(void) {
    gl_Position = vec4(in_position, 1.0);
    ex_color = in_color;
}
