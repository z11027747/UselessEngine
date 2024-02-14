
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"

layout (push_constant) uniform Push {
    mat4 model;
} push;

layout (location = 0) in vec3 inPositionOS;

void main() {
    DirectionLightUBO directionLight = globalUBO.directionLight;
    gl_Position = directionLight.projection * directionLight.view * push.model * vec4(inPositionOS, 1.0);
}