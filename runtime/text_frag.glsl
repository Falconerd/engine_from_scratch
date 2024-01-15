// #version 140
// in vec2 v_uvs;
// in vec4 v_color;

// out vec4 color;

// uniform sampler2D msdf;

// float median(float r, float g, float b) {
//     return max(min(r, g), min(max(r, g), b));
// }

// // TODO: Make these uniforms.
// float px_range = 4.0;
// vec2 texture_size = vec2(388.0, 388.0);
// // vec2 shadow_offset = vec2(1.0, 1.0);
// // float shadow_width = 1.0;
// // vec4 shadow_color = vec4(0.0, 0.0, 0.0, 1.0);

// float screen_px_range() {
//     vec2 unit_range = vec2(px_range) / texture_size;
//     vec2 screen_tex_size = vec2(1.0) / fwidth(v_uvs);
//     return max(0.5 * dot(unit_range, screen_tex_size), 1.0);
// }

// void main() {
//     vec2 flipped_uvs = vec2(v_uvs.x, 1.f - v_uvs.y);
//     vec3 msd = texture(msdf, flipped_uvs).rgb;
//     float sd = median(msd.r, msd.g, msd.b);
//     float scrpixdist = screen_px_range() * (sd - 0.5);
//     float opacity = clamp(scrpixdist + 0.5, 0.0, 1.0);
//     color = vec4(v_color.rgb, opacity);
// }


#version 140
in vec2 v_uvs;
in vec4 v_color;

out vec4 color;

uniform sampler2D msdf;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float px_range = 4.0;
vec2 texture_size = vec2(388.0, 388.0);

float screen_px_range() {
    vec2 unit_range = vec2(px_range) / texture_size;
    vec2 screen_tex_size = vec2(1.0) / fwidth(v_uvs);
    return max(0.5 * dot(unit_range, screen_tex_size), 1.0);
}

vec4 blur_sample(vec2 uv, float radius) {
    vec4 sum = vec4(0.0);
    int sampleNum = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 sampleUV = uv + vec2(x, y) * radius;
            vec3 msd = texture(msdf, sampleUV).rgb;
            float sd = median(msd.r, msd.g, msd.b);
            float scrpixdist = screen_px_range() * (sd - 0.5);
            float opacity = clamp(scrpixdist + 0.5, 0.0, 1.0);
            sum += vec4(v_color.rgb, opacity);
            sampleNum++;
        }
    }
    return sum / float(sampleNum);
}

void main() {
    vec2 flipped_uvs = vec2(v_uvs.x, 1.f - v_uvs.y);
    // Blur radius in terms of texture coordinates, adjust as needed
    float blurRadius = 1.0 / texture_size.x;
    color = blur_sample(flipped_uvs, blurRadius);
}