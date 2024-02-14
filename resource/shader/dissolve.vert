
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"

layout (push_constant) uniform Push {
    mat4 model;
} push;

layout (location = 0) in vec3 positionOS;
layout (location = 1) in vec2 uv0;

layout (location = 0) out vec2 fragUV0;

void main() {
    CameraUBO camera = globalUBO.camera;

    fragUV0 = uv0;

    gl_Position = camera.projection * camera.view * push.model * vec4(positionOS, 1.0);
}