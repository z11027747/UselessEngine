#version 450

struct CameraUBO {
    vec3 pos;
    mat4 view;
    mat4 projection;
};
struct DirectionLightUBO {
    vec3 dir;
    mat4 view;
    mat4 projection;
    vec3 ambient;
    vec3 color;
};
struct PointLight {
    vec3 pos;
    mat4 view;
    mat4 projection;
    vec3 color;
    vec3 clq;
};
layout (set = 0, binding = 0) uniform GlobalUBO {
    CameraUBO camera;
    DirectionLightUBO directionLight;
    PointLight pointLights[4];
    int activePointLights;
} globalUBO;

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

float CalcShadow() {
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

vec3 CalcHalfLambert(vec3 baseCol, vec3 normalWS, vec3 lightDir, float intensity) {
    float NdotL = clamp(dot(normalWS, lightDir), 0, 1);
    return baseCol * NdotL * intensity;
}

vec3 SafeNormalize(vec3 normal) {
    float magSq = dot(normal, normal);
    if (magSq == 0) {
        return vec3(0.0);
    }
    return normalize(normal);
}

vec3 CalcBlingPhone(vec3 baseCol, vec3 N, vec3 lightDir, float shininess, float intensity) {
    CameraUBO camera = globalUBO.camera;

    vec3 viewDir = normalize(camera.pos - positionWS);
    vec3 halfDir = SafeNormalize(viewDir + lightDir);
    float NdotH = clamp(dot(N, halfDir), 0, 1);
    return baseCol * pow(NdotH, shininess) * intensity;
}

vec3 CalcDirectionLight(vec3 baseCol, vec3 N, float shadowAtten) {
    DirectionLightUBO directionLight = globalUBO.directionLight;

    vec3 lightDir = normalize(directionLight.dir);
    vec3 lightAmbient = directionLight.ambient;
    vec3 lightColor = directionLight.color;

    float diffuseIntensity = materialUBO.params.x;
    // float specualrShininess = materialUBO.params.y;
    // float specularIntensity = materialUBO.params.z;

    vec3 diffuse = CalcHalfLambert(baseCol * lightColor, N, lightDir, diffuseIntensity);
    vec3 specular = vec3(0.0); //CalcBlingPhone(vec3(1.0), N, lightDir, specualrShininess, specularIntensity);

    return lightAmbient + (diffuse + specular) * shadowAtten;
}

vec3 CalcPointLight(int i, vec3 baseCol, vec3 N, float shadowAtten) {
    PointLight pointLight = globalUBO.pointLights[i];

    vec3 lightDir = normalize(pointLight.pos - positionWS);
    vec3 lightColor = pointLight.color;

    float dist = distance(pointLight.pos, positionWS);
    float atten = 1.0 /
        (pointLight.clq.x + pointLight.clq.y * dist + pointLight.clq.z * dist * dist);

    float diffuseIntensity = materialUBO.params.x;
    float specualrShininess = materialUBO.params.y;
    float specularIntensity = materialUBO.params.z;

    vec3 diffuse = CalcHalfLambert(baseCol * lightColor, N, lightDir, diffuseIntensity);
    vec3 specular = CalcBlingPhone(lightColor, N, lightDir, specualrShininess, specularIntensity);

    return (diffuse + specular) * atten * shadowAtten;
}

void main() {
    vec3 baseCol = texture(albedo, uv0).rgb;

    vec3 calcNormalMap = texture(normalMap, uv0).xyz * 2 - vec3(1.0);
    vec3 calcNormalWS = vec3(dot(tangentMat0, calcNormalMap), dot(tangentMat1, calcNormalMap), dot(tangentMat2, calcNormalMap));
    calcNormalWS = normalize(calcNormalWS);

    float shadowAtten = CalcShadow();

    vec3 directionLightCol = CalcDirectionLight(baseCol, calcNormalWS, shadowAtten);

    vec3 pointLightsCol = vec3(0.0);
    int activePointLights = globalUBO.activePointLights;
    for (int i = 0; i < activePointLights; i++) {
        pointLightsCol += CalcPointLight(i, baseCol, calcNormalWS, 1.0);
    }

    // outColor = vec4(directionLightCol, 1.0);
    outColor = vec4((directionLightCol + pointLightsCol) * color, 1.0);
}