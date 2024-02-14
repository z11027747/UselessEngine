#version 450
#extension GL_GOOGLE_include_directive : enable
#include "../include/global_ubo.glsl"

layout (set = 1, binding = 0) uniform sampler2DShadow shadowMap;
layout (input_attachment_index = 0, set = 1, binding = 1) uniform subpassInput positionAttachment;
layout (input_attachment_index = 1, set = 1, binding = 2) uniform subpassInput normalAttachment;
layout (input_attachment_index = 2, set = 1, binding = 3) uniform subpassInput colorAttachment;
layout (input_attachment_index = 3, set = 1, binding = 4) uniform subpassInput materialAttachment;
layout (input_attachment_index = 3, set = 1, binding = 5) uniform subpassInput pointlightAttachment;

layout (location = 0) out vec4 outColor;

float CalcShadow(vec4 positionLS) {
    vec4 positionNDCLS = positionLS / positionLS.w;

    if (abs(positionNDCLS.x) > 1.0 ||
        abs(positionNDCLS.y) > 1.0 ||
        abs(positionNDCLS.z) > 1.0) {
        return 1.0;
    } else {
        //pcf
        ivec2 texSize = textureSize(shadowMap, 0);
        float scale = 1.5;
        float dx = scale * 1.0 / float(texSize.x);
        float dy = scale * 1.0 / float(texSize.y);

        int PCF_SIZE = 3;
        int pcfSizeMinus1 = PCF_SIZE - 1;
        float kernelSize = 2.0 * pcfSizeMinus1 + 1.0;
        float numSamples = kernelSize * kernelSize;

        float shadowCount = 0.0;
        vec2 shadowMapCoord = vec2(positionNDCLS.xy * 0.5 + 0.5);

        for (int x = -pcfSizeMinus1; x <= pcfSizeMinus1; x++) {
            for (int y = -pcfSizeMinus1; y <= pcfSizeMinus1; y++) {
                vec2 pcfCoordinate = shadowMapCoord + vec2(dx * x, dy * y);
                vec3 pcfCoordinatePlusReference = vec3(pcfCoordinate, positionNDCLS.z);

                shadowCount += texture(shadowMap, pcfCoordinatePlusReference).r;
            }
        }

        return mix(0.2, 1.0, shadowCount / numSamples);
    }
}

vec3 CalcHalfLambert(vec3 baseCol, vec3 N, vec3 L, float intensity) {
    float NdotL = clamp(dot(N, L), 0, 1);
    return baseCol * NdotL * intensity;
}

vec3 SafeNormalize(vec3 normal) {
    float magSq = dot(normal, normal);
    if (magSq == 0) {
        return vec3(0.0);
    }
    return normalize(normal);
}

vec3 CalcBlingPhone(vec3 baseCol, vec3 V, vec3 N, vec3 L, float shininess, float intensity) {
    vec3 H = SafeNormalize(V + L);
    float NdotH = clamp(dot(N, H), 0, 1);
    return baseCol * pow(NdotH, shininess) * intensity;
}

vec3 CalcDirectionLight(vec3 baseCol, vec3 V, vec3 N, float shadowAtten, vec4 materialParams) {
    DirectionLightUBO directionLight = globalUBO.directionLight;

    vec3 lightDir = normalize(directionLight.dir);
    vec3 lightAmbient = directionLight.ambient;
    vec3 lightColor = directionLight.color;

    float diffuseIntensity = materialParams.x;
    float specualrShininess = materialParams.y;
    float specularIntensity = materialParams.z;

    vec3 diffuse = CalcHalfLambert(baseCol * lightColor, N, lightDir, diffuseIntensity);
    vec3 specular = CalcBlingPhone(vec3(1.0), V, N, lightDir, specualrShininess, specularIntensity);

    return lightAmbient + (diffuse + specular) * shadowAtten;
}

void main() {
    CameraUBO camera = globalUBO.camera;
    DirectionLightUBO directionLight = globalUBO.directionLight;

    vec3 positionWS = subpassLoad(positionAttachment).xyz;
    vec3 normalWS = subpassLoad(normalAttachment).xyz;
    vec3 baseCol = subpassLoad(colorAttachment).rgb;
    vec4 materialParams = subpassLoad(materialAttachment);

    vec4 positionLS = directionLight.projection * directionLight.view * vec4(positionWS, 1.0);
    float shadowAtten = CalcShadow(positionLS);

    vec3 viewDir = normalize(camera.pos - positionWS);
    vec3 directionLightCol = CalcDirectionLight(baseCol, viewDir, normalWS, shadowAtten, materialParams);

    vec3 pointLightsCol = subpassLoad(pointlightAttachment).rgb;

    outColor = vec4(directionLightCol + pointLightsCol, 1.0);
}