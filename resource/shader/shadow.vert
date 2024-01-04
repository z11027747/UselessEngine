#version 450

layout(set = 0, binding = 0) uniform GlobalUBO {
    vec3 cameraPos;
    mat4 cameraView;
    mat4 cameraProjection;
    vec3 directionLightPos;
    vec3 directionLightCol;
} globalUBO;

layout(push_constant) uniform Push {
    mat4 model;
} push;

layout(location = 0) in vec3 inPositionOS;

layout(location = 0) out float outDepth; 

void main() {
	gl_Position = globalUBO.cameraProjection * globalUBO.cameraView * push.model * vec4(inPositionOS, 1.0);
    outDepth = gl_Position.z;
}