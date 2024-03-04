const float PI = 3.14159265359;

//Lo = ∫π【 (kd*albedo*(1/PI) + ks*(D*F*G)/4(ωo·N)(ωi·L)) * Li(p,ωi) * N·ωi 】dωi

//L: p点微表面的radiance

//漫反射BRDF：
//  brdf = 1/π
//延半球均匀漫反射，入射光辐射度是 Li，平均每个方向是 Li/2π，跟入射角度没关系
//  Lo = ∫π ( brdf * Li * cosθ) dθ
//  Lo = π * brdf * Li （半球积分）
//  1 = π * brdf （能量守恒）
float Diffuse_BRDF() {
    return 1 / PI;
}

//镜面反射BRDF：
//  brdf = (D*F*G)/4(ωo*N)(ωi*N)

// D 法线分布函数 
float D_GGX(float NdotH, float roughness) {
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float denom = NdotH * NdotH * (alpha2 - 1.0) + 1.0;
    return (alpha2) / (PI * denom * denom);
}
// G 几何函数 
float G_SchlicksmithGGX(float NdotL, float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float GL = NdotL / (NdotL * (1.0 - k) + k);
    float GV = NdotV / (NdotV * (1.0 - k) + k);
    return GL * GV;
}
// F 菲涅尔方程
vec3 F_Schlick(float cosTheta, vec3 F0) {
    // return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
    return F0 + (1.0 - F0) * pow((1.0 - cosTheta), 5.0);
}
vec3 F_Schlick_WithRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 Specular_BRDF(float NdotL, float NdotV, float NdotH, vec3 F, float roughness) {
    float rroughness = max(0.05, roughness);

    float D = D_GGX(NdotH, rroughness);
    float G = G_SchlicksmithGGX(NdotL, NdotV, rroughness);

    return D * F * G / (4.0 * NdotL * NdotV + 0.0001);
}

// ========================

vec3 CalcReflection_PBR(samplerCube cubeMap, vec3 R, float roughness) {
    const float MAX_REFLECTION_LOD = 11.0; // todo: param/const
    float lod = roughness * MAX_REFLECTION_LOD;
    float lodf = floor(lod);
    float lodc = ceil(lod);
    vec3 a = textureLod(cubeMap, R, lodf).rgb;
    vec3 b = textureLod(cubeMap, R, lodc).rgb;
    return mix(a, b, lod - lodf);
}

//点光 明确知道所有可能的入射光线方向我们知道只有x个方向个入射光线会影响片段的着色，不用积分球数值解
vec3 CalcPointLight_PBR(int i, vec3 albedo, vec3 P, vec3 N, vec3 V, vec3 L, vec4 materialParams) {
    PointLightUBO pointLight = globalUBO.pointLights[i];

    float roughness = materialParams.x;
    float metallic = materialParams.y;

    // vec3 V = normalize(camera.pos.xyz - P);
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);

    float distance = length(pointLight.pos.xyz - P);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = pointLight.color.rgb * attenuation * 5;

    // 0°入射角的反射率 绝缘体大概是0.04
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = F_Schlick(NdotV, F0);

    float diffuseBRDF = Diffuse_BRDF();
    vec3 specularBRDF = Specular_BRDF(NdotL, NdotV, NdotH, F, roughness);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    // return (kD * albedo * diffuseBRDF) * radiance * NdotL;
    // return ( specularBRDF) * radiance * NdotL;
    return (kD * albedo * diffuseBRDF + specularBRDF) * radiance * NdotL;
}