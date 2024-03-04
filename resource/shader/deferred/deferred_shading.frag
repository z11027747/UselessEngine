
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "../include/global_ubo.glsl"
#include "../include/light.glsl"
#include "../include/shadow.glsl"

layout (set = 1, binding = 0) uniform sampler2DShadow shadowMap;
layout (input_attachment_index = 0, set = 1, binding = 1) uniform subpassInput positionAttachment;
layout (input_attachment_index = 1, set = 1, binding = 2) uniform subpassInput normalAttachment;
layout (input_attachment_index = 2, set = 1, binding = 3) uniform subpassInput colorAttachment;
layout (input_attachment_index = 3, set = 1, binding = 4) uniform subpassInput materialAttachment;
layout (input_attachment_index = 3, set = 1, binding = 5) uniform subpassInput pointlightAttachment;

layout (location = 0) out vec4 outColor;

void main() {
    CameraUBO camera = globalUBO.camera;
    DirectionLightUBO directionLight = globalUBO.directionLight;

    vec3 positionWS = subpassLoad(positionAttachment).xyz;
    vec3 normalWS = subpassLoad(normalAttachment).xyz;
    vec3 baseCol = subpassLoad(colorAttachment).rgb;
    vec4 materialParams = subpassLoad(materialAttachment);

    vec4 positionLS = directionLight.projection * directionLight.view * vec4(positionWS, 1.0);
   
    float shadowAtten = CalcShadow(positionLS, shadowMap);

    vec3 viewDir = normalize(camera.pos.xyz - positionWS);
    vec3 directionLightCol = CalcDirectionLight(baseCol, viewDir, normalWS, shadowAtten, materialParams);

    vec3 pointLightsCol = subpassLoad(pointlightAttachment).rgb;

    outColor = vec4(directionLightCol + pointLightsCol, 1.0);
}