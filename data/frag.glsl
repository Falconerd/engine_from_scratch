#version 140
in vec3 ex_color;
out vec4 frag_color;

void main(void) {
    frag_color = vec4(ex_color, 1.0);
}
