#version 450

//uniform
layout(set = 1, binding = 0) uniform samplerCube samplerCubeMap;

//in
layout(location = 0) in vec3 inUVW;

//out
layout(location = 0) out vec4 outFragColor;

void main() {
	outFragColor = texture(samplerCubeMap, inUVW);
}