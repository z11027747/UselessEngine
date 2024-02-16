
#version 450

layout (set = 0, binding = 0) uniform sampler2D gBufferPosition;
layout (set = 0, binding = 1) uniform sampler2D gBufferNormal;
layout (set = 0, binding = 2) uniform SSAOUBO {
    vec4 samples[64];
    vec4 noises[16];
} ssaoUBO;

layout (location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0);
}