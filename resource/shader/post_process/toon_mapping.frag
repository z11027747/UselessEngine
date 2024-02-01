#version 450

layout (push_constant) uniform Push {
    vec4 params;//Brightness+Saturation+Intensity
} push;

layout (set = 0, binding = 0) uniform sampler2D blitImage;

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 outColor;

float Luma(vec3 col) {
    return dot(col, vec3(0.2125, 0.7154, 0.0721));
}

void main() {
    vec3 col = texture(blitImage, uv).rgb;

    float brightness = push.params.x;
    float saturation = push.params.y;
    float contrast = push.params.z;

    col = col * brightness;
    col = mix(vec3(Luma(col)), col, saturation);
    col = mix(vec3(0.5), col, contrast);

    outColor = vec4(col, 1.0);
}