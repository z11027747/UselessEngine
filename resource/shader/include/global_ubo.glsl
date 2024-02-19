
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
    // mat4 view;
    // mat4 projection;
    vec3 color;
    vec3 clq;
};
struct SpotLight {
    vec3 dir;
    vec3 pos;
    // mat4 view;
    // mat4 projection;
    vec3 color;
    vec4 cutOff;
};
layout (set = 0, binding = 0) uniform GlobalUBO {
    CameraUBO camera;
    DirectionLightUBO directionLight;
    PointLight pointLights[256];
    int activePointLights;
    SpotLight spotLights[4];
    int activeSpotLights;
    float currTime;
} globalUBO;