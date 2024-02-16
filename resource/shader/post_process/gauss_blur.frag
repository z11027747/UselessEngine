
#version 450

layout (set = 1, binding = 0) uniform sampler2D blitImage;

layout (push_constant) uniform Push {
    vec4 params;//scale+strength+direction
} push;

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 outColor;

void main() {
    float scale = push.params.x;
    float strength = push.params.y;
    float direction = push.params.z;

    float enabled = push.params.w;
    if (enabled > 0) {
        float weight[5];
        weight[0] = 0.227027;
        weight[1] = 0.1945946;
        weight[2] = 0.1216216;
        weight[3] = 0.054054;
        weight[4] = 0.016216;

        vec2 tex_offset = 1.0 / textureSize(blitImage, 0) * scale;
        vec3 result = texture(blitImage, uv).rgb * weight[0];
        for (int i = 1; i < 5; ++i) {
            if (direction == 1) {
                // H
                result += texture(blitImage, uv + vec2(tex_offset.x * i, 0.0)).rgb * weight[i] * strength;
                result += texture(blitImage, uv - vec2(tex_offset.x * i, 0.0)).rgb * weight[i] * strength;
            } else {
                // V 
                result += texture(blitImage, uv + vec2(0.0, tex_offset.y * i)).rgb * weight[i] * strength;
                result += texture(blitImage, uv - vec2(0.0, tex_offset.y * i)).rgb * weight[i] * strength;
            }
        }

        outColor = vec4(result, 1.0);
    } else {
        vec3 col = texture(blitImage, uv).rgb;
        outColor = vec4(col, 1.0);
    }
}