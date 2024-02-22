const float PI = 3.14159265359;

// 法线分布函数 
float D_GGX(float dotNH, float roughness) {
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float denom = dotNH * dotNH * (alpha2 - 1.0) + 1.0;
    return (alpha2) / (PI * denom * denom);
}

// 几何函数 
float G_SchlicksmithGGX(float dotNL, float dotNV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float GL = dotNL / (dotNL * (1.0 - k) + k);
    float GV = dotNV / (dotNV * (1.0 - k) + k);
    return GL * GV;
}

// 菲涅尔方程 --------------
vec3 F_Schlick(float cosTheta, vec3 F0) {
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
    return F;
}