#version 140
in vec2 v_uvs;
in vec4 v_color;

out vec4 color;

uniform sampler2D msdf;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec3 flipped_uvs = vec3(v_uvs, 0);
    vec2 pos = flipped_uvs.xy;
    vec3 dist = texture(msdf, flipped_uvs.xy).rgb;
    float sig_dist = median(dist.r, dist.g, dist.b);
    float w = fwidth(sig_dist);
    float opacity = smoothstep(0.5 - w, 0.5 + w, sig_dist);
    color = vec4(v_color.rgb, opacity);
}
