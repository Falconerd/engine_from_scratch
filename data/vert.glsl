#version 310
in vec4 vp;
void main() {
    gl_Position = vec4(vp, 1.0);
}
