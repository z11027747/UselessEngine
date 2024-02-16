#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/image/image_comp.h"

namespace Render
{
    struct PostProcess final
    {
        inline static std::string type{"Render::PostProcess"};

        glm::vec4 toonMappingParams{1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec4 gaussBlurParams{1.0f, 1.5f, 0.0f, 1.0f};
        glm::vec4 bloomParams{0.4f, 1.0f, 0.0f, 1.0f};
    };

    constexpr int ssaoKernelSize = 64;
    constexpr int ssaoNoiseDim = 4;
    struct PostProcess_SSAOUBO final
    {
        glm::vec4 samples[ssaoKernelSize];
        glm::vec4 noiseValues[ssaoNoiseDim * ssaoNoiseDim];
    };
}