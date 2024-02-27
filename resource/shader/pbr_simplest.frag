
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
    DirectionLightUBO directionLight = globalUBO.directionLight;
    PointLight pointLights[256] = globalUBO.pointLights;
    int activePointLights = globalUBO.activePointLights;

    vec3 albedo = color;
    float roughness = materialUBO.params.x;
    float metallic = materialUBO.params.y;

    vec3 N = normalize(normalWS);
    vec3 V = normalize(camera.pos - positionWS);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo = vec3(0.0);

    //directionLight
    {
        vec3 L = normalize(-directionLight.dir);
        vec3 H = normalize(V + L);

        vec3 radiance = directionLight.color;

        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);

        float litPercentage = 1.0; //TODO
        Lo += (kD * albedo / PI + specular) * radiance * NdotL * litPercentage;
    }

    for (int i = 0; i < activePointLights; i++) {
        PointLight pointLight = pointLights[i];

        vec3 L = normalize(pointLight.pos - positionWS);
        vec3 H = normalize(V + L);

        float distance = length(pointLight.pos - positionWS);
        float attenuation = 1.0 / (distance * distance);

        vec3 radiance = pointLight.color * attenuation;

        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = clamp(dot(N, L), 0.0, 1.0);

        Lo += (kD * color / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = directionLight.ambient;
    vec3 finalColor = ambient + Lo;

    // HDR tonemapping
    finalColor = finalColor / (finalColor + vec3(1.0));
    // Gamma correct
    finalColor = pow(finalColor, vec3(1.0 / 2.2));

    outColor = vec4(albedo * finalColor, 1.0);
}