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
};
struct PointLight {
    vec3 pos;
    mat4 view;
    mat4 projection;
    vec3 color;
    vec3 clq;
};
layout (set = 0, binding = 0) uniform GlobalUBO {
    CameraUBO camera;
    DirectionLightUBO directionLight;
    PointLight pointLights[128];
    int activePointLights;
} globalUBO;

layout (push_constant) uniform Push {
    mat4 model;
} push;

layout (location = 0) in vec3 inPositionOS;

void main() {
    DirectionLightUBO directionLight = globalUBO.directionLight;
    gl_Position = directionLight.projection * directionLight.view * push.model * vec4(inPositionOS, 1.0);
}