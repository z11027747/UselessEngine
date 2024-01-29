#version 450

layout(set = 1, binding = 0) uniform MaterialUBO {
    vec4 params; //col rgba
} materialUBO;

layout(location = 0) in vec3 inColor;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 col = materialUBO.params;
    outColor = vec4(col.rgb * inColor, col.a);
}