
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"

layout (push_constant) uniform Push {
    mat4 model;
} push;

layout (location = 0) in vec3 positionOS;
layout (location = 1) in vec3 normalOS;
layout (location = 2) in vec3 color;

layout (location = 0) out vec3 fragPositionWS;
layout (location = 1) out vec3 fragNormalWS;
layout (location = 2) out vec3 fragColor;

void main() {
    CameraUBO camera = globalUBO.camera;

    fragPositionWS = (push.model * vec4(positionOS, 1.0)).xyz;
    fragNormalWS = (push.model * vec4(normalOS, 0.0)).xyz;
    fragColor = color;

    gl_Position = camera.projection * camera.view * vec4(fragPositionWS, 1.0);
}