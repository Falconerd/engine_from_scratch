#version 140
in vec3 v_uvs;
in vec4 v_color;

out vec4 color;

uniform sampler2DArray msdf;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    color = v_color;
    // vec3 flipped_v_uvs = vec3(v_uvs.x, 1.0 - v_uvs.y, v_uvs.z);
    // vec2 pos = flipped_v_uvs.xy;
    // vec3 sample = texture(msdf, flipped_v_uvs).rgb;
    // ivec2 sz = textureSize(msdf, 0).xy;
    // float dx = dFdx(pos.x) * sz.x; 
    // float dy = dFdy(pos.y) * sz.y;
    // // float to_pixels = 8.0 * inversesqrt(dx * dx + dy * dy);
    // float sig_dist = median(sample.r, sample.g, sample.b);
    // float w = fwidth(sig_dist);
    // float opacity = smoothstep(0.5 - w, 0.5 + w, sig_dist);
    // color = vec4(v_color.rgb, opacity);
}
