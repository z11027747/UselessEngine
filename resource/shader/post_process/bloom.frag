
#version 450

layout (push_constant) uniform Push {
    mat4 params;//Threshold+Intensity
} push;

layout (input_attachment_index = 0, set = 1, binding = 0) uniform subpassInput gaussBlurAttachment;

layout (location = 0) out vec4 outColor;

float Luma(vec3 col) {
    return dot(col, vec3(0.2125, 0.7154, 0.0721));
}

void main() {
    float lumaThresold = push.params[0].x;
    float intensity = push.params[0].y;

    float enabled = push.params[0].w;
    if (enabled > 0) {
        vec3 blurCol = subpassLoad(gaussBlurAttachment).rgb;
        float luma = clamp(Luma(blurCol) - lumaThresold, 0, 1);
        outColor = vec4(vec3(blurCol * luma * intensity), 1.0);
    } else {
        outColor = vec4(vec3(0.0), 1.0);
    }
}