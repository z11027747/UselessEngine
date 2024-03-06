
#version 450
// #extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"
#include "./include/light_pbr.glsl"

layout (set = 1, binding = 0) uniform sampler2D albedoMap;
layout (set = 1, binding = 1) uniform sampler2D normalMap;
layout (set = 1, binding = 2) uniform sampler2D roughnessMap;
layout (set = 1, binding = 3) uniform sampler2D metallicMap;
layout (set = 1, binding = 4) uniform sampler2D BRDFLUTMap;
layout (set = 1, binding = 5) uniform samplerCube skyboxCubeMap;

layout (set = 1, binding = 6) uniform MaterialUBO {
    vec4 params0; //useReflection
} materialUBO;

layout (location = 0) in vec3 positionWS;
layout (location = 1) in vec3 normalWS;
layout (location = 2) in vec3 tangentWS;
layout (location = 3) in vec3 bitangentWS;
layout (location = 4) in vec2 uv0;

layout (location = 0) out vec4 outColor;

void main() {
    vec3 albedo = textureLinear(albedoMap, uv0).rgb;
    float roughness = texture(roughnessMap, uv0).r;
    float metallic = texture(metallicMap, uv0).r;

    vec3 normalTS = texture(normalMap, uv0).xyz * 2 - vec3(1.0);
    vec3 normalWS = mat3(normalize(tangentWS), normalize(bitangentWS), normalize(normalWS)) * normalTS;

    vec3 P = positionWS;
    vec3 N = normalize(normalWS);
    vec3 V = normalize(globalUBO.camera.pos.xyz - positionWS);
    vec3 R = reflect(-V, N);

    float NdotV = max(dot(N, V), 0.0);
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < globalUBO.activePointLights; i++) {
        PointLightUBO pointLight = globalUBO.pointLights[i];
        vec3 L = normalize(pointLight.pos.xyz - P);
        Lo += CalcPointLight2_PBR(i, albedo, P, N, V, L, F0, roughness, metallic);
    }

    vec3 ambient = vec3(0.0);

    bool useRelection = (materialUBO.params0.x > 0.0);
    if (useRelection) {
        vec3 reflection = CalcReflection_PBR(skyboxCubeMap, R, roughness);

        vec3 F = F_SchlickR(NdotV, F0, roughness);

        // float irrdianceLod = materialUBO.params0.y;
        // vec3 irradiance = texture(skyboxCubeMap, N, irrdianceLod).rgb;
        // vec3 diffuse = irradiance * albedo;

        // vec3 kD = 1.0 - F;
        // kD *= 1.0 - metallic;
        // ambient += kD * diffuse;

        vec2 BRDF = texture(BRDFLUTMap, vec2(NdotV, roughness)).rg;
        vec3 specular = reflection * (F * BRDF.x + BRDF.y);

        ambient += specular;
    }

    vec3 finalColor = ambient + Lo;

	// Tone mapping
    // finalColor = Uncharted2Tonemap(finalColor * 4.5);
    // finalColor = finalColor * (1.0 / Uncharted2Tonemap(vec3(11.2)));	
    // Gamma correct
    finalColor = pow(finalColor, vec3(1.0 / 2.2));

    outColor = vec4(finalColor, 1.0);
}