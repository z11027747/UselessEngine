#version 450

layout (push_constant) uniform Push {
    vec4 params;//
} push;

layout (set = 0, binding = 0) uniform sampler2D colorAttachment;

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 outColor;

void main() {
    vec3 e = texture(colorAttachment, vec2(uv.x, uv.y)).rgb;
    outColor = vec4(e, 1.0);
}