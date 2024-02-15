
#version 450
#include "../include/depth.glsl"

layout (set = 1, binding = 0) uniform sampler2D albedo;
layout (set = 1, binding = 1) uniform sampler2D normalMap;

layout (set = 1, binding = 2) uniform MaterialUBO {
    vec4 params; //light diffuseIntensity+specualrShininess+specularIntensity
} materialUBO;

layout (location = 0) in vec4 positionWS; //w is depth
layout (location = 1) in vec3 tangentMat0;
layout (location = 2) in vec3 tangentMat1;
layout (location = 3) in vec3 tangentMat2;
layout (location = 4) in vec3 color;
layout (location = 5) in vec2 uv0;

layout (location = 0) out vec4 outPosition;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outColor;
layout (location = 3) out vec4 outMaterial;

void main() {
    vec3 baseCol = texture(albedo, uv0).rgb;

    vec3 calcNormalMap = texture(normalMap, uv0).xyz * 2 - vec3(1.0);
    vec3 calcNormalWS = vec3(dot(tangentMat0, calcNormalMap), dot(tangentMat1, calcNormalMap), dot(tangentMat2, calcNormalMap));
    calcNormalWS = normalize(calcNormalWS);

    outPosition = vec4(positionWS.xyz, 1.0);
    outNormal = vec4(normalize(calcNormalWS), 1.0);
    outColor = vec4(baseCol * color, 1.0);

    outMaterial.xyz = materialUBO.params.xyz;
    outMaterial.w = LinearizeDepth(positionWS.w);
}