

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

    vec3 lightDir = normalize(directionLight.dir);
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