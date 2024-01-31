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
        static void Create(Context *, std::shared_ptr<EngineObject>);
        static void Destroy(Context *, std::shared_ptr<EngineObject>);
    };
}