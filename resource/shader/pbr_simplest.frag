
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"
#include "./include/light_pbr.glsl"

layout (set = 1, binding = 0) uniform MaterialUBO {
    vec4 params; //pbr roughness+metallic
} materialUBO;

layout (location = 0) in vec3 positionWS;
layout (location = 1) in vec3 normalWS;
layout (location = 2) in vec3 color;

layout (location = 0) out vec4 outColor;

void main() {
    CameraUBO camera = globalUBO.camera;
    PointLight pointLights[256] = globalUBO.pointLights;
    int activePointLights = globalUBO.activePointLights;

    float roughness = materialUBO.params.x;
    float metallic = materialUBO.params.y;

    vec3 N = normalize(normalWS);
    vec3 V = normalize(camera.pos - positionWS);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < activePointLights; i++) {
        vec3 L = normalize(pointLights[i].pos - positionWS);
        vec3 H = normalize(V + L);

        float dotNV = clamp(dot(N, V), 0.0, 1.0);
        float dotNL = clamp(dot(N, L), 0.0, 1.0);
        // float dotLH = clamp(dot(L, H), 0.0, 1.0);
        float dotNH = clamp(dot(N, H), 0.0, 1.0);

        if (dotNL > 0.0) {
            float rroughness = max(0.05, roughness);

            float D = D_GGX(dotNH, roughness);
            vec3 F0 = mix(vec3(0.04), color, metallic);
            vec3 F = F_Schlick(dotNV, F0);
            float G = G_SchlicksmithGGX(dotNL, dotNV, rroughness);

            vec3 spec = D * F * G / (4.0 * dotNL * dotNV);
            Lo += spec * dotNL * pointLights[i].color;
        }
    }

    vec3 ambient = color * 0.02;
    vec3 color = ambient + Lo;

	// Gamma correct
    color = pow(color, vec3(1.0 / 2.2));

    outColor = vec4(color, 1.0);
}