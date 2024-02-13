#version 450

layout (set = 1, binding = 0) uniform sampler2D depth;

layout (location = 0) in vec3 color;
layout (location = 1) in vec2 positionSS;
layout (location = 2) in vec3 positionVS;

layout (location = 0) out vec4 outColor;

float near = 0.1;
float far = 50.0;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    float depth = texture(depth, positionSS).r;
    float depthZ = LinearizeDepth(depth);
    float viewZ = -positionVS.z;

    float foam = 1 - clamp((depthZ - viewZ), 0, 1);

    vec3 finalColor = color.rgb + vec3(foam);

    outColor = vec4(finalColor, 0.8);
}