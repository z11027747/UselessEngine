#pragma once

#include <glm/glm.hpp>

namespace Render
{
    struct DirectionLight final
    {
        glm::vec3 color;
        glm::vec4 params;
        
        bool hasShadow;
    };
}