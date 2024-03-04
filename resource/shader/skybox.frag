
#version 450

layout(set = 1, binding = 0) uniform samplerCube cubeMap;

layout(set = 1, binding = 1) uniform MaterialUBO {
	vec4 params0; //lodLevel(x)
} materialUBO;

layout(location = 0) in vec3 inUVW;

layout(location = 0) out vec4 outColor;

void main() {
	float lodLevel = floor(materialUBO.params0.x);
	outColor = textureLod(cubeMap, inUVW, lodLevel);
}