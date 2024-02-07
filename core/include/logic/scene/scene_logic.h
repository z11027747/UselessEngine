#pragma once

#include <string>
#include <memory>
#include <json/json11.hpp>

class Context;
class EngineObject;

namespace Logic
{
    class SceneLogic final
    {
    public:
        static void CreateMainCamera(Context *);
        static void CreateAxis(Context *);
    };

    class SceneJsonLogic final
    {
    public:
        static std::shared_ptr<EngineObject> ParseLine(Context *, const json11::Json &);
        static std::string SerializeEO(Context *, std::shared_ptr<EngineObject>);
    };
}