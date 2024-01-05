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
layout(location = 1) in vec3 inNormalOS;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inUV0;

layout(location = 0) out vec3 fragPositionWS;
layout(location = 1) out vec3 fragNormalWS;
layout(location = 2) out vec3 fragColor;
layout(location = 3) out vec2 fragUV0;

void main() {
    gl_Position = globalUBO.camera.projection * globalUBO.camera.view * push.model * vec4(inPositionOS, 1.0);
	fragPositionWS = mat3(push.model) * inPositionOS;
	fragNormalWS = mat3(push.model) * inNormalOS;
    fragColor = inColor;
    fragUV0 = inUV0;
}