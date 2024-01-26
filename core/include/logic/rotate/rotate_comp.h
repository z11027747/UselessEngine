#pragma once

#include <glm/glm.hpp>
#include <memory>

class EngineObject;

namespace Logic
{
    struct RotateAround final
    {
        std::shared_ptr<EngineObject> targetEO;
        glm::vec3 offset;
        float angleSpeed;
        float currAngle;
    };
}
