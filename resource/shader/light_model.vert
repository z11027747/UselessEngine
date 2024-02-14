#version 450
#extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"

layout (push_constant) uniform Push {
    mat4 model;
} push;

layout (location = 0) in vec3 positionOS;
layout (location = 1) in vec3 normalOS;
layout (location = 2) in vec3 tangentOS;
layout (location = 3) in vec3 color;
layout (location = 4) in vec2 uv0;

layout (location = 0) out vec3 fragPositionWS;
layout (location = 1) out vec3 fragNormalWS;
layout (location = 2) out vec3 fragTangentMat0;
layout (location = 3) out vec3 fragTangentMat1;
layout (location = 4) out vec3 fragTangentMat2;
layout (location = 5) out vec3 fragColor;
layout (location = 6) out vec2 fragUV0;
layout (location = 7) out vec4 fragPositionLS;

void main() {
    CameraUBO camera = globalUBO.camera;
    DirectionLightUBO directionLight = globalUBO.directionLight;

    fragPositionWS = (push.model * vec4(positionOS, 1.0)).xyz;

    //TODO 懒得算M的逆矩阵的转置矩阵了
    fragNormalWS = (push.model * vec4(normalOS, 0.0)).xyz;
    vec3 fragTangentWS = (push.model * vec4(tangentOS, 0.0)).xyz;

    float orient = sign(push.model[0][0] * push.model[1][1] * push.model[2][2]);

    vec3 fragBitangentWS = cross(fragTangentWS, fragNormalWS) * orient;
    fragTangentMat0 = vec3(fragTangentWS.x, fragBitangentWS.x, fragNormalWS.x);
    fragTangentMat1 = vec3(fragTangentWS.y, fragBitangentWS.y, fragNormalWS.y);
    fragTangentMat2 = vec3(fragTangentWS.z, fragBitangentWS.z, fragNormalWS.z);

    fragColor = color;
    fragUV0 = uv0;
    fragPositionLS = directionLight.projection * directionLight.view * push.model * vec4(positionOS, 1.0);

    gl_Position = camera.projection * camera.view * push.model * vec4(positionOS, 1.0);
}