
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "./include/global_ubo.glsl"
#include "./include/light.glsl"

layout (set = 1, binding = 1) uniform MaterialUBO {
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
layout (location = 1) out vec2 fragPositionSS;
layout (location = 2) out vec3 fragPositionVS;

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
    // DirectionLightUBO directionLight = globalUBO.directionLight;

    vec3 dir0 = tangentOS - positionOS;
    vec3 dir1 = normalOS - positionOS;
    vec3 normalOSAvg = normalize(cross(dir0, dir1));
    vec3 normalWSAvg = normalize((push.model * vec4(normalOSAvg, 0.0)).xyz);

    vec3 positionOSAvg = (positionOS + normalOS + tangentOS) / 3.0;
    vec3 positionWSAvg = (push.model * vec4(positionOSAvg, 1.0)).xyz;

    vec3 lightDir = normalize(vec3(-0.5, 0.7, -0.5));//normalize(directionLight.dir);
    vec3 viewDir = normalize(globalUBO.camera.pos - positionWSAvg);

    float diffuseIntensity = materialUBO.params_light.x;
    float specualrShininess = materialUBO.params_light.y;
    float specularIntensity = materialUBO.params_light.z;
    vec3 diffuse = CalcHalfLambert(vec3(1.0), normalWSAvg, lightDir, diffuseIntensity);
    vec3 specular = CalcBlingPhone(vec3(1.0), viewDir, normalWSAvg, lightDir, specualrShininess, specularIntensity);

    vec3 position = GerstnerWave(materialUBO.params_wave.w); //speed
    gl_Position = camera.projection * camera.view * push.model * vec4(position, 1.0);

    fragColor = (diffuse + specular) * color;

    vec2 positionNDC = gl_Position.xy / gl_Position.w;
    fragPositionSS = positionNDC * 0.5 + 0.5;
    fragPositionVS = (camera.view * push.model * vec4(positionOS, 1.0)).xyz;
}