#version 450

layout (push_constant) uniform Push {
    vec4 params;//Scale+Threshold+Intensity
} push;

layout (set = 0, binding = 0) uniform sampler2D blitImage;

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 outColor;

float Luma(vec3 col) {
    return dot(col, vec3(0.2125, 0.7154, 0.0721));
}

void main() {
    ivec2 texSize = textureSize(blitImage, 0);

    float scale = push.params.x;
    float dx = scale * 1.0 / float(texSize.x);
    float dy = scale * 1.0 / float(texSize.y);

    int mipLevel = 4;
    // Take 13 samples around current texel:
    // === ('e' is the current texel) ===
    // a - b - c
    // - j - k -
    // d - e - f
    // - l - m -
    // g - h - i
    vec3 a = texture(blitImage, vec2(uv.x - 2 * dx, uv.y + 2 * dy), mipLevel).rgb;
    vec3 b = texture(blitImage, vec2(uv.x, uv.y + 2 * dy), mipLevel).rgb;
    vec3 c = texture(blitImage, vec2(uv.x + 2 * dx, uv.y + 2 * dy), mipLevel).rgb;

    vec3 d = texture(blitImage, vec2(uv.x - 2 * dx, uv.y), mipLevel).rgb;
    vec3 e = texture(blitImage, vec2(uv.x, uv.y), mipLevel).rgb;
    vec3 f = texture(blitImage, vec2(uv.x + 2 * dx, uv.y), mipLevel).rgb;

    vec3 g = texture(blitImage, vec2(uv.x - 2 * dx, uv.y - 2 * dy), mipLevel).rgb;
    vec3 h = texture(blitImage, vec2(uv.x, uv.y - 2 * dy), mipLevel).rgb;
    vec3 i = texture(blitImage, vec2(uv.x + 2 * dx, uv.y - 2 * dy), mipLevel).rgb;

    vec3 j = texture(blitImage, vec2(uv.x - dx, uv.y + dy), mipLevel).rgb;
    vec3 k = texture(blitImage, vec2(uv.x + dx, uv.y + dy), mipLevel).rgb;
    vec3 l = texture(blitImage, vec2(uv.x - dx, uv.y - dy), mipLevel).rgb;
    vec3 m = texture(blitImage, vec2(uv.x + dx, uv.y - dy), mipLevel).rgb;

    vec3 blurColor = vec3(0);
    blurColor.rgb = e * 0.125;
    blurColor.rgb += (a + c + g + i) * 0.03125;
    blurColor.rgb += (b + d + f + h) * 0.0625;
    blurColor.rgb += (j + k + l + m) * 0.125;

    float lumaThresold = push.params.y;
    float luma = clamp(Luma(e) - lumaThresold, 0, 1);

    float intensity = push.params.z;

    vec3 col = texture(blitImage, uv).rgb;
    outColor = vec4(col + (blurColor * luma * intensity), 1.0);
}