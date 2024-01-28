
#pragma once

#include <memory>

class Context;
class EngineObject;

namespace Render
{
    class LightLogic final
    {
    public:
        static void OnAdd(Context *, std::shared_ptr<EngineObject>);
        static void OnRemove(Context *, std::shared_ptr<EngineObject>);
    };
}