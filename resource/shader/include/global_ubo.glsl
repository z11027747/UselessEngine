struct CameraUBO {
    vec4 pos; //xyz
    mat4 view;
    mat4 projection;
};
struct DirectionLightUBO {
    vec4 dir; //xyz
    mat4 view;
    mat4 projection;
    vec4 ambient; //rgb
    vec4 color; //rgb
};
struct PointLightUBO {
    vec4 pos; //xyz
    // mat4 view;
    // mat4 projection;
    vec4 color; //rgb
    vec4 clq; //rgb
};
struct SpotLightUBO {
    vec4 dir; //xyz
    vec4 pos; //xyz
    // mat4 view;
    // mat4 projection;
    vec4 color; //rgb
    vec4 cutOff;
};
layout(set = 0, binding = 0) uniform GlobalUBO {
    CameraUBO camera;
    DirectionLightUBO directionLight;
    PointLightUBO pointLights[256];
    SpotLightUBO spotLights[4];
    ivec4 lightParams; //activePointLights(x) activeSpotLights(y)
    vec4 timeParams; //currTime(x)
} globalUBO;