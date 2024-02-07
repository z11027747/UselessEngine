
#include "logic/scene/scene_system.h"
#include "logic/scene/scene_logic.h"
#include "define.hpp"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    std::shared_ptr<EngineObject> SceneJsonLogic::ParseLine(Context *context, const json11::Json &j)
    {
        auto jObj = j.object_items();

        auto eo = std::make_shared<EngineObject>();
        eo->name = jObj["name"].string_value();
        eo->active = jObj["active"].bool_value();
        eo->hideInHierarchy = jObj["hideInHierarchy"].bool_value();

        auto &components = jObj["components"].array_items();
        for (const auto &componentJObj : components)
        {
            auto &type = componentJObj["type"].string_value();
            auto component = EngineComponent::JsonParse(type, componentJObj);
            if (component != nullptr)
                context->AddComponent(eo, type, component);
        }

        return eo;
    }

    std::string SceneJsonLogic::SerializeEO(Context *context, std::shared_ptr<EngineObject> eo)
    {
        json11::Json::array componentJObjArr = {};

        auto &componentMap = eo->componentMap;
        for (const auto &kv : componentMap)
        {
            auto type = kv.first;
            auto &component = kv.second;
            auto componentJObj = EngineComponent::JsonSerialize(type, component);
            componentJObjArr.push_back(componentJObj);
        }

        json11::Json::object jObj = {
            {"name", eo->name},
            {"active", eo->active},
            {"hideInHierarchy", eo->hideInHierarchy},
            {"components", componentJObjArr}};

        auto j = json11::Json(jObj);
        return j.dump();
    }
}
