
#version 450
#extension GL_GOOGLE_include_directive : enable
#include "../include/global_ubo.glsl"
#include "../include/depth.glsl"

const int kernelSize = 64;
const int noiseDim = 4;
const float radius = 0.5;
const float bias = 0.025;

layout (set = 1, binding = 0) uniform sampler2D gBufferPosition;
layout (set = 1, binding = 1) uniform sampler2D gBufferNormal;
layout (set = 1, binding = 2) uniform sampler2D gBufferDepth;
layout (set = 1, binding = 3) uniform sampler2D noise;
layout (set = 1, binding = 4) uniform SSAOUBO {
    vec4 samples[kernelSize];
    vec4 noiseValues[noiseDim * noiseDim];
} ssaoUBO;

layout (push_constant) uniform Push {
    mat4 params; //
} push;

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 outColor;

void main() {
    CameraUBO camera = globalUBO.camera;

    vec3 positionWS = texture(gBufferPosition, uv).xyz;
    vec3 normalWS = normalize(texture(gBufferNormal, uv).xyz);

    vec3 positionVS = (camera.view * vec4(positionWS, 1.0)).xyz;
    vec3 normalVS = normalize((camera.view * vec4(normalWS, 0.0)).xyz);

    //0.1 1920/4=480  
    // ivec2 texDim = textureSize(gBufferDepth, 0);
    // int noiseX = int(fract(uv.x * texDim.x / noiseDim) * noiseDim);
    // int noiseY = int(fract(uv.y * texDim.y / noiseDim) * noiseDim);
    int noiseX = int(fract(uv.x * noiseDim));
    int noiseY = int(fract(uv.y * noiseDim));
    int noiseIndex = noiseDim * noiseY + noiseX;
    vec3 randomVec = ssaoUBO.noiseValues[noiseIndex].xyz;
    // randomVec = randomVec * 2.0 - 1.0;
    randomVec = normalize(randomVec);

    // outColor = vec4(vec3(randomVec), 1.0);
    // return;

	// Create TBN matrix
    vec3 tangentVS = normalize(randomVec - normalVS * dot(randomVec, normalVS));
    vec3 bitangentVS = cross(tangentVS, normalVS);
    mat3 TBN = mat3(tangentVS, bitangentVS, normalVS);

    vec3 samplePosVS = positionVS + (TBN * vec3(1.0, 0.0, 0.0)) * radius;
    // outColor = vec4(vec3(Linearize01Depth(-samplePosVS.z)), 1.0);

    vec4 offset = camera.projection * vec4(samplePosVS, 1.0);
    offset.xyz /= offset.w;
    offset.xyz = offset.xyz * 0.5 + 0.5;

    float sampleDepth = texture(gBufferDepth, uv).w;
    // outColor = vec4(vec3(LinearizeDepth(-sampleDepth)), 1.0);
    // return;

    if (sampleDepth >= samplePosVS.z) {
        outColor = vec4(vec3(0.0, 1.0, 0.0), 1.0);
        return;
    }

    outColor = vec4(vec3(0.0, 0.0, 0.0), 1.0);
    return;

	// Calculate occlusion value
    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++) {
        vec3 samplePosVS = TBN * ssaoUBO.samples[i].xyz;
        samplePosVS = positionVS + samplePosVS * radius;

		// project
        vec4 offset = vec4(samplePosVS, 1.0);
        offset = camera.projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(gBufferDepth, offset.xy).w;

        // float rangeCheck = smoothstep(0.0, 1.0, radius / abs(positionVS.z - sampleDepth));
        // occlusion += (sampleDepth >= samplePosVS.z + bias ? 1.0 : 0.0) * rangeCheck;

        occlusion += (sampleDepth >= -samplePosVS.z ? 1.0 : 0.0);
    }

    occlusion = 1.0 - (occlusion / float(kernelSize));
    outColor = vec4(vec3(occlusion), 1.0);
}