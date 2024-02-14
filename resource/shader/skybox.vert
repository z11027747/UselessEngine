
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"

layout (push_constant) uniform Push {
    mat4 model;
} push;

layout (location = 0) in vec3 inPositionOS;

layout (location = 0) out vec3 outUVW;

void main() {
    gl_Position = globalUBO.camera.projection * globalUBO.camera.view * push.model * vec4(inPositionOS, 1.0);
    outUVW = inPositionOS;
}