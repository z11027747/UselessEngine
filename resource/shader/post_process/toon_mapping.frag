
#version 450

layout (set = 1, binding = 0) uniform sampler2D blitImage;

layout (push_constant) uniform Push {
    mat4 params;//Brightness+Saturation+Intensity
} push;

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 outColor;

float Luma(vec3 col) {
    return dot(col, vec3(0.2125, 0.7154, 0.0721));
}

void main() {
    vec3 col = texture(blitImage, uv).rgb;

    float brightness = push.params[0].x;
    float saturation = push.params[0].y;
    float contrast = push.params[0].z;

    float enabled = push.params[0].w;
    if (enabled > 0) {
        col = col * brightness;
        col = mix(vec3(Luma(col)), col, saturation);
        col = mix(vec3(0.5), col, contrast);
    }

    outColor = vec4(col, 1.0);
}