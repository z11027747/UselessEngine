
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"

layout (push_constant) uniform Push {
    mat4 model;
} push;

layout (location = 0) in vec3 positionOS;
layout (location = 1) in vec3 normalOS;
layout (location = 2) in vec3 tangentOS;
layout (location = 3) in vec2 uv0;

layout (location = 0) out vec3 fragPositionWS;
layout (location = 1) out vec3 fragNormalWS;
layout (location = 2) out vec3 fragTangentWS;
layout (location = 3) out vec3 fragBitangentWS;
layout (location = 4) out vec2 fragUV0;
layout (location = 5) out vec3 fragTest;

void main() {
    CameraUBO camera = globalUBO.camera;

    fragPositionWS = (push.model * vec4(positionOS, 1.0)).xyz;
    fragNormalWS = mat3(push.model) * normalOS;
    fragTangentWS = mat3(push.model) * tangentOS;
    fragBitangentWS = cross(fragNormalWS, fragTangentWS);
    fragUV0 = uv0;
    fragTest = tangentOS;

    gl_Position = camera.projection * camera.view * vec4(fragPositionWS, 1.0);
}