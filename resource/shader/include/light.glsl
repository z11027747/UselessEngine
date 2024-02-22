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

    vec3 lightDir = normalize(-directionLight.dir);
    vec3 lightAmbient = directionLight.ambient;
    vec3 lightColor = directionLight.color;

    float diffuseIntensity = materialParams.x;
    float specualrShininess = materialParams.y;
    float specularIntensity = materialParams.z;

    vec3 diffuse = CalcHalfLambert(baseCol * lightColor, N, lightDir, diffuseIntensity);
    vec3 specular = CalcBlingPhone(vec3(1.0), V, N, lightDir, specualrShininess, specularIntensity);

    return lightAmbient + (diffuse + specular) * shadowAtten;
}

vec3 CalcPointLight(int i, vec3 baseCol, vec3 V, vec3 N, vec3 P, float shadowAtten, vec4 materialParams) {
    PointLight pointLight = globalUBO.pointLights[i];

    vec3 lightColor = pointLight.color;
    vec3 lightPos = pointLight.pos;

    float dist = distance(lightPos, P);
    float atten = 1.0 /
        (pointLight.clq.x + pointLight.clq.y * dist + pointLight.clq.z * dist * dist);

    float diffuseIntensity = materialParams.x;
    float specualrShininess = materialParams.y;
    float specularIntensity = materialParams.z;

    vec3 L = normalize(lightPos - P);
    vec3 diffuse = CalcHalfLambert(baseCol * lightColor, N, L, diffuseIntensity);
    vec3 specular = CalcBlingPhone(lightColor, V, N, L, specualrShininess, specularIntensity);

    return (diffuse + specular) * atten * shadowAtten;
}

vec3 CalcSpotLight(int i, vec3 baseCol, vec3 V, vec3 N, vec3 P, float shadowAtten, vec4 materialParams) {
    SpotLight spotLight = globalUBO.spotLights[i];

    vec3 L = normalize(spotLight.pos - P);

    vec3 lightDir = normalize(spotLight.dir);
    float lightCutOff = spotLight.cutOff.x;
    float lightOuterCutOff = spotLight.cutOff.y;

    float theta = dot(-L, lightDir);
    if (theta > lightCutOff) {
        vec3 lightColor = spotLight.color;

        float diffuseIntensity = materialParams.x;
        float specualrShininess = materialParams.y;
        float specularIntensity = materialParams.z;

        vec3 diffuse = CalcHalfLambert(baseCol * lightColor, N, L, diffuseIntensity);
        vec3 specular = CalcBlingPhone(vec3(1.0), V, N, L, specualrShininess, specularIntensity);

        vec3 result = (diffuse + specular) * shadowAtten;

        float intensity = clamp((theta - lightCutOff) / (lightCutOff - lightOuterCutOff), 0.0, 1.0);
        result *= intensity;

        return result;
    } else {
        return vec3(0.0);
    }
}

// PBR ================

const float PI = 3.14159265359;

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}