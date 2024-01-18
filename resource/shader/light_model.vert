#version 450

struct CameraUBO {
    vec3 pos;
    mat4 view;
    mat4 projection;
};

struct DirectionLightUBO {
    vec3 dir;
    mat4 view;
    mat4 projection;
    vec3 ambient;
    vec3 color;
    vec4 params; //TODO
};

layout(set = 0, binding = 0) uniform GlobalUBO {
    CameraUBO camera;
    DirectionLightUBO directionLight;
} globalUBO;

layout(push_constant) uniform Push {
    mat4 model;
} push;

layout(location = 0) in vec3 positionOS;
layout(location = 1) in vec3 normalOS;
layout(location = 2) in vec3 tangentOS;
layout(location = 3) in vec3 color;
layout(location = 4) in vec2 uv0;

layout(location = 0) out vec3 fragPositionWS;
layout(location = 1) out vec3 fragNormalWS;
layout(location = 2) out vec3 fragTangentWS;
layout(location = 3) out vec3 fragTangentMat0;
layout(location = 4) out vec3 fragTangentMat1;
layout(location = 5) out vec3 fragTangentMat2;
layout(location = 6) out vec3 fragColor;
layout(location = 7) out vec2 fragUV0;
layout(location = 8) out vec4 fragPositionLS;

void main() {
    CameraUBO camera = globalUBO.camera;
    DirectionLightUBO directionLight = globalUBO.directionLight;

    fragPositionWS = (push.model * vec4(positionOS, 1.0)).xyz;
    fragNormalWS = (push.model * vec4(normalOS, 1.0)).xyz;

    fragTangentWS = (push.model * vec4(tangentOS, 1.0)).xyz;
    vec3 fragBitangentWS = cross(fragNormalWS, fragTangentWS);
    fragTangentMat0 = vec3(fragTangentWS.x, fragBitangentWS.x, fragNormalWS.x);
    fragTangentMat1 = vec3(fragTangentWS.y, fragBitangentWS.y, fragNormalWS.y);
    fragTangentMat2 = vec3(fragTangentWS.z, fragBitangentWS.z, fragNormalWS.z);

    fragColor = color;
    fragUV0 = uv0;
    fragPositionLS = directionLight.projection * directionLight.view * vec4(fragPositionWS, 1.0);

    gl_Position = camera.projection * camera.view * vec4(fragPositionWS, 1.0);
}