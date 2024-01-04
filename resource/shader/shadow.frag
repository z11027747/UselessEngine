#version 450

layout(location = 0) in float outDepth;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(outDepth);
}