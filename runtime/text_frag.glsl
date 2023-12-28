#version 140
in vec2 v_uvs;
in vec4 v_color;

out vec4 color;

uniform sampler2D msdf;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

// TODO: Make these uniforms.
float px_range = 4.0;
vec2 texture_size = vec2(388.0, 388.0);
// vec2 shadow_offset = vec2(1.0, 1.0);
// float shadow_width = 1.0;
// vec4 shadow_color = vec4(0.0, 0.0, 0.0, 1.0);

float screen_px_range() {
    vec2 unit_range = vec2(px_range) / texture_size;
    vec2 screen_tex_size = vec2(1.0) / fwidth(v_uvs);
    return max(0.5 * dot(unit_range, screen_tex_size), 1.0);
}

void main() {
    vec3 msd = texture(msdf, v_uvs).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float scrpixdist = screen_px_range() * (sd - 0.5);
    float opacity = clamp(scrpixdist + 0.5, 0.0, 1.0);
    color = vec4(v_color.rgb, opacity);
}
