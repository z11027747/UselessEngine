
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"
#include "./include/light.glsl"
#include "./include/shadow.glsl"

layout (set = 1, binding = 0) uniform sampler2DShadow shadowMap;
layout (set = 1, binding = 1) uniform sampler2D albedo;
layout (set = 1, binding = 2) uniform sampler2D normalMap;

layout (set = 1, binding = 3) uniform MaterialUBO {
    vec4 params; //light diffuseIntensity+specualrShininess+specularIntensity
} materialUBO;

layout (location = 0) in vec3 positionWS;
layout (location = 1) in vec3 normalWS;
layout (location = 2) in vec3 tangentMat0;
layout (location = 3) in vec3 tangentMat1;
layout (location = 4) in vec3 tangentMat2;
layout (location = 5) in vec3 color;
layout (location = 6) in vec2 uv0;
layout (location = 7) in vec4 positionLS;

layout (location = 0) out vec4 outColor;

void main() {
    vec3 baseCol = texture(albedo, uv0).rgb;

    vec3 calcNormalMap = texture(normalMap, uv0).xyz * 2 - vec3(1.0);
    vec3 calcNormalWS = vec3(dot(tangentMat0, calcNormalMap), dot(tangentMat1, calcNormalMap), dot(tangentMat2, calcNormalMap));
    calcNormalWS = normalize(calcNormalWS);

    float shadowAtten = CalcShadow(positionLS, shadowMap);

    vec3 viewDir = normalize(globalUBO.camera.pos - positionWS);
    vec3 directionLightCol = CalcDirectionLight(baseCol, viewDir, calcNormalWS, shadowAtten, materialUBO.params);

    vec3 pointLightsCol = vec3(0.0);
    int activePointLights = globalUBO.activePointLights;
    for (int i = 0; i < activePointLights; i++) {
        pointLightsCol += CalcPointLight(i, baseCol, viewDir, calcNormalWS, positionWS, shadowAtten, materialUBO.params);
    }

    outColor = vec4((directionLightCol + pointLightsCol) * color, 1.0);
}