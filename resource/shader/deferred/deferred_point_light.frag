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
    PointLight pointLights[256];
    int activePointLights;
} globalUBO;

layout (set = 1, binding = 0) uniform sampler2DShadow shadowMap;
layout (input_attachment_index = 0, set = 1, binding = 1) uniform subpassInput positionAttachment;
layout (input_attachment_index = 1, set = 1, binding = 2) uniform subpassInput normalAttachment;
layout (input_attachment_index = 2, set = 1, binding = 3) uniform subpassInput colorAttachment;
layout (input_attachment_index = 3, set = 1, binding = 4) uniform subpassInput materialAttachment;

layout (location = 0) out vec4 outColor;

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

vec3 CalcPointLight(int i, vec3 baseCol, vec3 V, vec3 N, vec3 P, float shadowAtten, vec4 materialParams) {
    PointLight pointLight = globalUBO.pointLights[i];

    vec3 lightColor = pointLight.color;
    vec3 L = normalize(pointLight.pos - P);

    float dist = distance(pointLight.pos, P);
    float atten = 1.0 /
        (pointLight.clq.x + pointLight.clq.y * dist + pointLight.clq.z * dist * dist);

    float diffuseIntensity = materialParams.x;
    float specualrShininess = materialParams.y;
    float specularIntensity = materialParams.z;

    vec3 diffuse = CalcHalfLambert(baseCol * lightColor, N, L, diffuseIntensity);
    vec3 specular = CalcBlingPhone(lightColor, V, N, L, specualrShininess, specularIntensity);

    return (diffuse + specular) * atten * shadowAtten;
}

void main() {
    CameraUBO camera = globalUBO.camera;
    DirectionLightUBO directionLight = globalUBO.directionLight;

    vec3 positionWS = subpassLoad(positionAttachment).xyz;
    vec3 normalWS = subpassLoad(normalAttachment).xyz;
    vec3 baseCol = subpassLoad(colorAttachment).rgb;
    vec4 materialParams = subpassLoad(materialAttachment);

    vec4 positionLS = directionLight.projection * directionLight.view * vec4(positionWS, 1.0);

    vec3 viewDir = normalize(camera.pos - positionWS);
    
    vec3 pointLightsCol = vec3(0.0);
    int activePointLights = globalUBO.activePointLights;
    for (int i = 0; i < activePointLights; i++) {
        pointLightsCol += CalcPointLight(i, baseCol, viewDir, normalWS, positionWS, 1.0, materialParams);
    }

    outColor = vec4(pointLightsCol, 1.0);
}