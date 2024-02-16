
#version 450

layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput SSAOAttachment;
layout (input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput toonMappingAttachment;
layout (input_attachment_index = 2, set = 0, binding = 2) uniform subpassInput gaussBlurAttachment;
layout (input_attachment_index = 3, set = 0, binding = 3) uniform subpassInput bloomAttachment;

layout (location = 0) out vec4 outColor;

void main() {
    vec3 toonMappingCol = subpassLoad(toonMappingAttachment).rgb;
    // vec3 gaussBlurCol = subpassLoad(gaussBlurAttachment).rgb;
    vec3 bloomCol = subpassLoad(bloomAttachment).rgb;

    outColor = vec4(toonMappingCol + bloomCol, 1.0);
}