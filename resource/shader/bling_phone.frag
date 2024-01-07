#version 450

struct CameraUBO
{
    vec3 pos;
    mat4 view;
    mat4 projection;
};

struct DirectionLightUBO 
{
    mat4 view;
    mat4 projection;
	vec3 dir;
	vec3 col;
	vec4 params;
};

layout(set = 0, binding = 0) uniform GlobalUBO {
    CameraUBO camera;
    DirectionLightUBO directionLight;
} globalUBO;

layout(set = 1, binding = 0) uniform sampler2D baseSampler;
layout(set = 1, binding = 1) uniform sampler2DShadow shadowSampler;

layout(location = 0) in vec3 inPositionWS;
layout(location = 1) in vec3 inNormalWS;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inUV;
layout(location = 4) in vec4 inPositionLS;

layout(location = 0) out vec4 outColor;

vec3 CalcDirectionLight()
{
    vec3 ambient = vec3(0.4);

    float diffuseIntensity = globalUBO.directionLight.params.x;

    vec3 lightDir = normalize(globalUBO.directionLight.dir);
    vec3 normalDir = normalize(inNormalWS);
    vec3 diffuse = max(0.0, dot(normalDir, lightDir)) * diffuseIntensity * globalUBO.directionLight.col;
    
    float specualrShininess = globalUBO.directionLight.params.y;
    float specularIntensity = globalUBO.directionLight.params.z;

    vec3 viewDir = normalize(globalUBO.camera.pos - inPositionWS);
    vec3 halfDir = normalize(viewDir + lightDir);
    vec3 specular = pow(max(0.0, dot(normalDir, halfDir)), specualrShininess) * specularIntensity * vec3(1.0);

    return diffuse + specular;
}

vec3 CalcShadow()
{
    vec3 shadowUVW = inPositionLS.xyz / inPositionLS.w;
    shadowUVW.xy = shadowUVW.xy * 0.5 + 0.5;

    float shadowCmp = texture(shadowSampler, shadowUVW);
    return vec3(shadowCmp);
}

void main() {
    vec3 baseCol = texture(baseSampler, inUV).rgb;
    vec3 directionLightCol = CalcDirectionLight();
    vec3 shadowColor = CalcShadow();
    outColor = vec4(shadowColor,1.0);
    return;
    outColor = vec4(inColor * baseCol * directionLightCol * shadowColor, 1.0);
}