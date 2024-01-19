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

layout(set = 0, binding = 0) uniform GlobalUBO {
    CameraUBO camera;
    DirectionLightUBO directionLight;
} globalUBO;

layout(set = 1, binding = 0) uniform sampler2DShadow shadowSampler;
layout(set = 1, binding = 1) uniform sampler2D albedoSampler;
layout(set = 1, binding = 2) uniform sampler2D specularSampler;
layout(set = 1, binding = 3) uniform sampler2D normalMapSampler;

layout(set = 1, binding = 4) uniform LightModelUBO {
    vec4 params;
} lightModelUBO;

layout(location = 0) in vec3 positionWS;
layout(location = 1) in vec3 normalWS;
layout(location = 2) in vec3 tangentWS;
layout(location = 3) in vec3 tangentMat0;
layout(location = 4) in vec3 tangentMat1;
layout(location = 5) in vec3 tangentMat2;
layout(location = 6) in vec3 color;
layout(location = 7) in vec2 uv0;
layout(location = 8) in vec4 positionLS;

layout(location = 0) out vec4 outColor;

float CalcShadow() {
    vec3 shadowUVW = positionLS.xyz / positionLS.w;
    shadowUVW.xy = shadowUVW.xy * 0.5 + 0.5;

    float shadowCmp = texture(shadowSampler, shadowUVW);
    return mix(1, 0.2, shadowUVW.z - shadowCmp);//  step(shadowUVW.z, shadowCmp);
}

vec3 CalcDirectionLight(float shadowAtten) {
    CameraUBO camera = globalUBO.camera;
    DirectionLightUBO directionLight = globalUBO.directionLight;

    vec3 ambient = directionLight.ambient;

    // vec3 fragNormalWS = normalize(normalWS);
    vec3 calcNormalDir = texture(normalMapSampler, uv0).xyz * 2 - vec3(1.0);
    vec3 fragNormalWS = vec3(dot(tangentMat0, calcNormalDir), dot(tangentMat1, calcNormalDir), dot(tangentMat2, calcNormalDir));
    fragNormalWS = normalize(fragNormalWS);

    float diffuseIntensity = lightModelUBO.params.x;

    //lambert
    vec3 baseCol = texture(albedoSampler, uv0).rgb;
    vec3 lightDir = normalize(directionLight.dir);
    vec3 diffuse = baseCol * max(0.0, dot(fragNormalWS, lightDir)) * diffuseIntensity * directionLight.color;

    //bling-phone
    float specualrShininess = lightModelUBO.params.y;
    float specularIntensity = lightModelUBO.params.z;

    vec3 viewDir = normalize(camera.pos - positionWS);
    vec3 halfDir = normalize(viewDir + lightDir);
    vec3 specular = pow(max(0.0, dot(fragNormalWS, halfDir)), specualrShininess) * specularIntensity * vec3(1.0);

    return ambient + (diffuse + specular) * shadowAtten;
}

void main() {
    float shadowAtten = CalcShadow();
    vec3 directionLightCol = CalcDirectionLight(shadowAtten);
    outColor = vec4(directionLightCol * color, 1.0);
}