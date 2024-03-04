
#version 450
// #extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"
#include "./include/light_pbr.glsl"

layout (set = 1, binding = 0) uniform MaterialUBO {
    vec4 params0; //roughness+metallic
    vec4 params1; //albedo(rgb)
} materialUBO;

layout (location = 0) in vec3 positionWS;
layout (location = 1) in vec3 normalWS;
layout (location = 2) in vec3 color;

layout (location = 0) out vec4 outColor;

void main() {
    vec3 albedo = materialUBO.params1.rgb;

    vec3 P = positionWS;
    vec3 N = normalize(normalWS);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < globalUBO.activePointLights; i++) {
        Lo += CalcPointLight_PBR(i, albedo, N, P, materialUBO.params0);
    }

    // vec3 ambient = directionLight.ambient.rgb;
    vec3 ambient = vec3(0.02) * albedo;
    vec3 finalColor = ambient + Lo;

	// Tone mapping
    finalColor = finalColor / (finalColor + vec3(1.0));
    // Gamma correct
    finalColor = pow(finalColor, vec3(1.0 / 2.2));

    outColor = vec4(finalColor, 1.0);
}