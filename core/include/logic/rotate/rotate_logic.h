#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "logic/rotate/rotate_comp.h"

class Context;
class EngineObject;

namespace Logic
{
    class RotateLogic final
    {
    public:
        static void OnAdd(Context *, std::shared_ptr<EngineObject>);
        static void OnRemove(Context *, std::shared_ptr<EngineObject>);

        static void BeginRotateAround(Context *,
                                      std::shared_ptr<EngineObject>,
                                      std::shared_ptr<EngineObject>, float);
        static void BeginRotateAround(Context *,
                                      std::shared_ptr<EngineObject>,
                                      std::shared_ptr<EngineObject>, const glm::vec3 &, float);
    };
}
