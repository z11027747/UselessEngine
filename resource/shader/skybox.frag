#version 450
layout(set = 0, binding = 0) uniform GlobalUBO {
    vec3 cameraPos;
    mat4 cameraView;
    mat4 cameraProjection;
    vec3 directionLightPos;
    vec3 directionLightCol;
} globalUBO;

layout(set = 1, binding = 0) uniform samplerCube samplerCubeMap;

layout(location = 0) in vec3 inUVW;

layout(location = 0) out vec4 outColor;

void main() {
	outColor = texture(samplerCubeMap, inUVW);
}