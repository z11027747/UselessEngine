
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "../include/global_ubo.glsl"

const int kernelSize = 64;
const int noiseDim = 4;

layout (set = 1, binding = 0) uniform sampler2D gBufferPosition;
layout (set = 1, binding = 1) uniform sampler2D gBufferNormal;
layout (set = 1, binding = 2) uniform sampler2D gBufferDepth;
layout (set = 1, binding = 3) uniform SSAOUBO {
    vec4 samples[kernelSize];
    vec4 noiseValues[noiseDim * noiseDim];
} ssaoUBO;

layout (push_constant) uniform Push {
    vec4 params; //radius+bias
} push;

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 outColor;

void main() {
    float radius = push.params.x;
    float bias = push.params.y;
    float enabled = push.params.w;
    if (enabled <= 0) {
        discard; //clearColor vec4(1.0)
    }

    CameraUBO camera = globalUBO.camera;

    vec3 positionWS = texture(gBufferPosition, uv).xyz;
    vec3 normalWS = normalize(texture(gBufferNormal, uv).xyz);

    vec3 positionVS = (camera.view * vec4(positionWS, 1.0)).xyz;
    vec3 normalVS = normalize((camera.view * vec4(normalWS, 0.0)).xyz);

    ivec2 texDim = textureSize(gBufferDepth, 0);
    int noiseX = int(fract(uv.x * texDim.x / noiseDim) * noiseDim);
    int noiseY = int(fract(uv.y * texDim.y / noiseDim) * noiseDim);
    // int noiseX = int(fract(uv.x * noiseDim));
    // int noiseY = int(fract(uv.y * noiseDim));
    int noiseIndex = noiseDim * noiseY + noiseX;
    vec3 randomVec = ssaoUBO.noiseValues[noiseIndex].xyz;
    randomVec = normalize(randomVec);

    vec3 tangentVS = normalize(randomVec - normalVS * dot(randomVec, normalVS));
    vec3 bitangentVS = cross(tangentVS, normalVS);
    mat3 TBN = mat3(tangentVS, bitangentVS, normalVS);

	// Calculate occlusion value
    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++) {
        vec3 samplePosVS = TBN * ssaoUBO.samples[i].xyz;
        samplePosVS = positionVS + samplePosVS * radius;

        vec4 samplePosNDC = camera.projection * vec4(samplePosVS, 1.0);
        samplePosNDC.xy /= samplePosNDC.w;
        samplePosNDC.xy = samplePosNDC.xy * 0.5 + 0.5;

        // !! pipeline viewport set !!
        samplePosNDC.y = 1 - samplePosNDC.y;

        float sampleViewZ = texture(gBufferDepth, samplePosNDC.xy).w;

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(positionVS.z - sampleViewZ));
        occlusion += (sampleViewZ >= samplePosVS.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / float(kernelSize));
    outColor = vec4(vec3(occlusion), 1.0);
}