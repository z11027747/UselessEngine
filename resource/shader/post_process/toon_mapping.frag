#version 450

layout (push_constant) uniform Push {
    vec4 params;//x+Threshold+Intensity+Scale
} push;

layout (set = 0, binding = 0) uniform sampler2D colorAttachment;

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 outColor;

float Luma(vec3 col) {
    return dot(col, vec3(0.2125, 0.7154, 0.0721));
}

void main() {
    ivec2 texSize = textureSize(colorAttachment, 0);

    float scale = push.params.y;
    float dx = scale * 1.0 / float(texSize.x);
    float dy = scale * 1.0 / float(texSize.y);

    // Take 13 samples around current texel:
    // === ('e' is the current texel) ===
    // a - b - c
    // - j - k -
    // d - e - f
    // - l - m -
    // g - h - i
    vec3 a = texture(colorAttachment, vec2(uv.x - 2 * dx, uv.y + 2 * dy)).rgb;
    vec3 b = texture(colorAttachment, vec2(uv.x, uv.y + 2 * dy)).rgb;
    vec3 c = texture(colorAttachment, vec2(uv.x + 2 * dx, uv.y + 2 * dy)).rgb;

    vec3 d = texture(colorAttachment, vec2(uv.x - 2 * dx, uv.y)).rgb;
    vec3 e = texture(colorAttachment, vec2(uv.x, uv.y)).rgb;
    vec3 f = texture(colorAttachment, vec2(uv.x + 2 * dx, uv.y)).rgb;

    vec3 g = texture(colorAttachment, vec2(uv.x - 2 * dx, uv.y - 2 * dy)).rgb;
    vec3 h = texture(colorAttachment, vec2(uv.x, uv.y - 2 * dy)).rgb;
    vec3 i = texture(colorAttachment, vec2(uv.x + 2 * dx, uv.y - 2 * dy)).rgb;

    vec3 j = texture(colorAttachment, vec2(uv.x - dx, uv.y + dy)).rgb;
    vec3 k = texture(colorAttachment, vec2(uv.x + dx, uv.y + dy)).rgb;
    vec3 l = texture(colorAttachment, vec2(uv.x - dx, uv.y - dy)).rgb;
    vec3 m = texture(colorAttachment, vec2(uv.x + dx, uv.y - dy)).rgb;

    vec3 blurColor = vec3(0);
    blurColor.rgb = e * 0.125;
    blurColor.rgb += (a + c + g + i) * 0.03125;
    blurColor.rgb += (b + d + f + h) * 0.0625;
    blurColor.rgb += (j + k + l + m) * 0.125;

    float lumaThresold = push.params.z;
    float luma = clamp(Luma(e) - lumaThresold, 0, 1);

    float intensity = push.params.w;
    outColor = vec4(e + (blurColor * luma * intensity), 1.0);
}