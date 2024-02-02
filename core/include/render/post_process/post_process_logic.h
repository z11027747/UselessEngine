#pragma once

#include <glm/glm.hpp>
#include <memory>

class EngineObject;
class Context;

namespace Render
{
    class PostProcessLogic final
    {
    public:
        static void OnAdd(Context *, std::shared_ptr<EngineObject>);
        static void OnRemove(Context *, std::shared_ptr<EngineObject>);
    };
}