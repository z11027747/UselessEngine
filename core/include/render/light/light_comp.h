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
        glm::vec3 clq{1.0f, 0.045f, 0.0075f};
        bool hasShadow{true};
    };

    struct SpotLight final
    {
        inline static std::string type{"Render::SpotLight"};
    };
}