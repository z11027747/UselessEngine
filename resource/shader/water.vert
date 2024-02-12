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
    float currTime;
} globalUBO;

layout (set = 1, binding = 0) uniform MaterialUBO {
    vec4 params_wave; //water Angle+Steepness01+length
    vec4 params_light; //specular shininess+intensity
} materialUBO;

layout (push_constant) uniform Push {
    mat4 model;
} push;

layout (location = 0) in vec3 positionOS;
layout (location = 1) in vec3 normalOS;
layout (location = 2) in vec3 tangentOS;
layout (location = 3) in vec3 color;

layout (location = 0) out vec3 fragColor;

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

vec3 GerstnerWave(float speed) {
    vec4 params_wave = materialUBO.params_wave;
    float time = globalUBO.currTime;

    float x = cos(params_wave.x);
    float y = sin(params_wave.x);
    float z = 2 * 3.14 / (params_wave.z / 20);//size
    float w = (params_wave.y / z);

    float dot = (x * positionOS.x + y * positionOS.z);
    float value = dot * z - time * speed;
    float amp = w;

    vec3 position = positionOS;
    position.x += x * (amp * cos(value));
    position.y += amp * sin(value);
    position.z += y * (amp * cos(value));

    return position;
}

void main() {
    CameraUBO camera = globalUBO.camera;
    DirectionLightUBO directionLight = globalUBO.directionLight;

    vec3 dir0 = tangentOS - positionOS;
    vec3 dir1 = normalOS - positionOS;
    vec3 normalOSAvg = normalize(cross(dir0, dir1));
    vec3 normalWSAvg = normalize((push.model * vec4(normalOSAvg, 0.0)).xyz);

    vec3 positionOSAvg = (positionOS + normalOS + tangentOS) / 3.0;
    vec3 positionWSAvg = (push.model * vec4(positionOSAvg, 1.0)).xyz;

    vec3 lightDir = normalize(directionLight.dir);
    vec3 viewDir = normalize(globalUBO.camera.pos - positionWSAvg);

    float diffuseIntensity = materialUBO.params_light.x;
    float specualrShininess = materialUBO.params_light.y;
    float specularIntensity = materialUBO.params_light.z;
    vec3 diffuse = CalcHalfLambert(vec3(1.0), normalWSAvg, lightDir, diffuseIntensity);
    vec3 specular = CalcBlingPhone(vec3(1.0), viewDir, normalWSAvg, lightDir, specualrShininess, specularIntensity);

    fragColor = (diffuse + specular) * color;

    vec3 position = GerstnerWave(materialUBO.params_wave.w); //speed
    gl_Position = camera.projection * camera.view * push.model * vec4(position, 1.0);
}