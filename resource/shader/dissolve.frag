
#version 450

layout (set = 1, binding = 0) uniform sampler2D albedo;
layout (set = 1, binding = 1) uniform sampler2D dissolve;
layout (set = 1, binding = 2) uniform sampler2D ramp;

layout (set = 1, binding = 3) uniform MaterialUBO {
    vec4 params; //dissolve clip+alpha
} materialUBO;

layout (location = 0) in vec2 uv0;

layout (location = 0) out vec4 outColor;

void main() {
    vec3 baseCol = texture(albedo, uv0).rgb;

    float dissolveClip = materialUBO.params.x;
    float dissolveAlpha = materialUBO.params.y;

    float dissolveR = texture(dissolve, uv0).r;
    if (dissolveR < dissolveClip) {
        discard;
    }

    float dissolveValue = clamp((dissolveR - dissolveClip) / dissolveAlpha, 0, 1);
    vec3 rampCol = texture(ramp, vec2(dissolveValue)).rgb;

    vec3 col = mix(rampCol, baseCol, dissolveValue);

    outColor = vec4(col, 1.0);
}