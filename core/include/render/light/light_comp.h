#pragma once

#include <glm/glm.hpp>

namespace Render
{
    struct DirectionLight final
    {
        glm::vec3 ambient{0.1f, 0.1f, 0.1f};
        glm::vec3 color;

        bool hasShadow{true};
    };
}