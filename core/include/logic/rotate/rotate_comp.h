#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

class EngineObject;

namespace Logic
{
    struct RotateAround final
    {
		inline static std::string type{"Logic::RotateAround"};

        std::shared_ptr<EngineObject> targetEO;
        glm::vec3 offset;
        float angleSpeed;
        float currAngle;
    };
}
