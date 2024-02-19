#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Render
{
    struct DirectionLight final
    {
        inline static std::string type{"Render::DirectionLight"};

        glm::vec3 ambient{0.1f, 0.1f, 0.1f};
        glm::vec3 color;

        bool hasShadow{true};
    };

    struct PointLight final
    {
        inline static std::string type{"Render::PointLight"};

        glm::vec3 color;

        // http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
        glm::vec3 clq{1.0f, 0.7f, 1.8f};
        bool hasShadow{true};
    };

    struct SpotLight final
    {
        inline static std::string type{"Render::SpotLight"};

        glm::vec3 color;
        glm::vec4 cutOff; // xy cos(rad), zw deg
        bool hasShadow{true};
    };
}