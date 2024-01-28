#pragma once

#include <glm/glm.hpp>
#include <memory>

class Context;
class EngineObject;

namespace Logic
{
    class MoveLogic final
    {
    public:
        static void OnAdd(Context *, std::shared_ptr<EngineObject>);
        static void OnRemove(Context *, std::shared_ptr<EngineObject>);

        static void BeginFollow(Context *,
                                std::shared_ptr<EngineObject>,
                                std::shared_ptr<EngineObject>, const glm::vec3 &);
    };
}
