#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "logic/move/move_comp.h"

class Context;
class EngineObject;

namespace Logic
{
    class MoveLogic final
    {
    public:
        static void BeginFollow(Context *,
                                std::shared_ptr<EngineObject>,
                                std::shared_ptr<EngineObject>, glm::vec3 &);
    };
}
