#version 450

//uniform
layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 view;
    mat4 projection;
} globalUBO;
layout(push_constant) uniform Push {
    mat4 model;
} push;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = globalUBO.projection * globalUBO.view * push.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}