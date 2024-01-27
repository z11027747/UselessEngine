#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Logic
{
    struct HitRay final 
    {
		inline static std::string type{"Logic::HitRay"};

        glm::vec3 origin;
        glm::vec3 direction;
    };
}