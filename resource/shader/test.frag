#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 2) uniform sampler2D imgSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(imgSampler, fragTexCoord) * vec4(fragColor, 1.0);
}