
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"
#include "./include/light.glsl"

layout (set = 1, binding = 0) uniform MaterialUBO {
    vec4 params; //pbr roughness+metallic
} materialUBO;

layout (location = 0) in vec3 positionWS;
layout (location = 1) in vec3 normalWS;
layout (location = 2) in vec3 color;

layout (location = 0) out vec4 outColor;

void main() {
    CameraUBO camera = globalUBO.camera;
    DirectionLightUBO directionLight = globalUBO.directionLight;
    PointLight pointLights[256] = globalUBO.pointLights;
    int activePointLights = globalUBO.activePointLights;

    float roughness = materialUBO.params.x;
    float metallic = materialUBO.params.y;

    vec3 albedo = color;

    vec3 N = normalize(normalWS);
    vec3 V = normalize(camera.pos - positionWS);

    vec3 Lo = vec3(0.0); //总辐射率

    for (int i = 0; i < activePointLights; i++) {
        vec3 L = normalize(pointLights[i].pos - positionWS);
        vec3 H = normalize(V + L);

        float distance = length(pointLights[i].pos - positionWS);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLights[i].color * attenuation; //点光辐射率

        //非金属表面，F0始终为0.04 （大多绝缘体在F0=0.04）
        //金属表面，根据初始的F0和表现金属属性的反射率进行线性插值
        vec3 F0 = vec3(0.04);
        F0 = mix(F0, albedo, metallic);

        //菲涅尔F
        vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
        //法线分布函数D 
        float D = DistributionGGX(N, H, roughness);
        //几何遮蔽函数G
        float G = GeometrySmith(N, V, L, roughness);

        vec3 nominator = D * F * G;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; //防止除0
        vec3 specular = nominator / denominator;

        vec3 kS = F; //高光反射，和菲涅尔相关
        vec3 kD = vec3(1.0) - kS; //漫反射

        //金属不会折射光线（没有漫反射）
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);

        //Fr(p, Wi, Wo) * L(p, Wi) * dot(N, Wi)
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = directionLight.ambient * albedo;
    vec3 color = ambient + Lo;

    //LDR->HDR + 伽马校正
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    outColor = vec4(color, 1.0);
}