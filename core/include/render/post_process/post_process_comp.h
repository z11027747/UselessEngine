#pragma once

#include <glm/glm.hpp>
#include <string>
#include "render/vk/pipeline/descriptor_comp.h"

namespace Render
{
    struct PostProcess final
    {
        inline static std::string type{"Render::PostProcess"};

        VkDescriptorSetLayout descriptorSetLayout;
        std::shared_ptr<Descriptor> descriptor;

        bool enabled{true};
        glm::vec4 bloomParams{1.0f, 5.0f, 0.5f, 1.0f};
        glm::vec4 toonMappingParams{1.0f, 1.0f, 1.0f, 1.0f};
    };
}