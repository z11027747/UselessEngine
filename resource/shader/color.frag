#version 450

layout(set = 1, binding = 0) uniform MaterialUBO {
    vec4 params; //col r+g+b
} materialUBO;

layout(location = 0) in vec3 inColor;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 col = materialUBO.params.xyz;
    outColor = vec4(col * inColor, 1.0);
}