#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <json/json11.hpp>

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

    class PostProcessJson final
    {
    public:
        static std::shared_ptr<void> Parse(const json11::Json &);
        static json11::Json Serialize(std::shared_ptr<void>);
    };
}