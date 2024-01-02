#version 450

layout(set = 0, binding = 0) uniform GlobalUBO {
    vec3 cameraPos;
    mat4 cameraView;
    mat4 cameraProjection;
    vec3 directionLightPos;
    vec3 directionLightCol;
    vec4 directionLightParams;
} globalUBO;

layout(set = 1, binding = 0) uniform sampler2D imgSampler;

layout(location = 0) in vec3 inPositionWS;
layout(location = 1) in vec3 inNormalWS;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec4 outColor;

vec3 CalcDirectionLight()
{
    vec3 ambient = vec3(0.4);

    float diffuseIntensity = globalUBO.directionLightParams.x;

    vec3 lightDir = normalize(globalUBO.directionLightPos);
    vec3 normalDir = normalize(inNormalWS);
    vec3 diffuse = max(0.0, dot(normalDir, lightDir)) * diffuseIntensity * globalUBO.directionLightCol;
    
    float specualrShininess = globalUBO.directionLightParams.y;
    float specularIntensity = globalUBO.directionLightParams.z;

    vec3 viewDir = normalize(globalUBO.cameraPos - inPositionWS);
    vec3 halfDir = normalize(viewDir + lightDir);
    vec3 specular = pow(max(0.0, dot(normalDir, halfDir)), specualrShininess) * specularIntensity * globalUBO.directionLightCol;

    return ambient + diffuse + specular;
}

void main() {
    vec3 baseCol = texture(imgSampler, inUV).rgb;
    vec3 directionLightCol = CalcDirectionLight();
    outColor = vec4(inColor * baseCol * directionLightCol, 1.0);
}