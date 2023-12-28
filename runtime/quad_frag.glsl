#version 140
in vec2 v_uvs;
in vec4 v_color;
out vec4 color;

uniform sampler2D tex;

void main() {
    color = texture(tex, v_uvs);
}

