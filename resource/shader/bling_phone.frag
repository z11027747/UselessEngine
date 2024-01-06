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
    vec3 specular = pow(max(0.0, dot(normalDir, halfDir)), specualrShininess) * specularIntensity * globalUBO.directionLight.col;

    return ambient + diffuse + specular;
}

vec3 CalcShadow()
{
    vec4 positionLS_Clip = globalUBO.directionLight.projection * globalUBO.directionLight.view * vec4(inPositionWS, 1.0);
    vec3 positionLS_NDC = positionLS_Clip.xyz / positionLS_Clip.w;

    float depth = positionLS_NDC.z;
    vec2 uv = positionLS_NDC.xy * 0.5 + 0.5;
    
    float shadowDepth = texture(shadowSampler, vec3(uv, depth));
    return vec3(1-(depth-shadowDepth));
}

vec3 CalcShadow_PCF()
{
    vec4 positionLS_Clip = globalUBO.directionLight.projection * globalUBO.directionLight.view * vec4(inPositionWS, 1.0);
    vec3 positionLS_NDC = positionLS_Clip.xyz / positionLS_Clip.w;

    float depth = positionLS_NDC.z;
    vec2 uv = positionLS_NDC.xy * 0.5 + 0.5;
            
    int samplerCount = 0;
    float samplerValue = 0.0;
    vec2 samplerTexelSize = 5 / vec2(1920.0, 1080.0);

    int pcfCount = 3;
    for (int x = -pcfCount; x <= pcfCount; x++)
    {
        for (int y = -pcfCount; y <= pcfCount; y++)
        {
            vec2 uv_pcf = uv + vec2(x, y)*samplerTexelSize;
            samplerCount += 1;
            samplerValue += texture(shadowSampler, vec3(uv_pcf, depth));
        }
    }

    float shadowDepth = samplerValue / samplerCount;

    return vec3(1-(depth-shadowDepth));
}

void main() {
    vec3 baseCol = texture(baseSampler, inUV).rgb;
    vec3 directionLightCol = CalcDirectionLight();
    vec3 shadowColor = CalcShadow();
    // outColor = vec4(shadowColor, 1.0);
    // return;
    outColor = vec4(inColor * baseCol * directionLightCol * shadowColor, 1.0);
}