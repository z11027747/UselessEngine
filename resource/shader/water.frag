#version 450

layout (set = 1, binding = 0) uniform MaterialUBO {
    vec4 params; //water
} materialUBO;

layout (location = 0) in vec3 color;

layout (location = 0) out vec4 outColor;

void main() {

    outColor = vec4(color, 0.9);
}