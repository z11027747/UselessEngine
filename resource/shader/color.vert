#version 450

struct CameraUBO
{
    vec3 pos;
    mat4 view;
    mat4 projection;
};

struct DirectionLightUBO 
{
    mat4 view;
    mat4 projection;
	vec3 dir;
	vec3 col;
	vec4 params;
};

layout(set = 0, binding = 0) uniform GlobalUBO {
    CameraUBO camera;
    DirectionLightUBO directionLight;
} globalUBO;

layout(push_constant) uniform Push {
    mat4 model;
} push;

layout(location = 0) in vec3 inPositionOS;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
	gl_Position = globalUBO.camera.projection * globalUBO.camera.view * push.model * vec4(inPositionOS, 1.0);
    fragColor = inColor;
}