#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <json/json11.hpp>
#include "common/reflection/type_json.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
    static std::shared_ptr<EngineObject> EOFromJson(Context *context, const json11::Json &jObj)
    {
        auto eo = std::make_shared<EngineObject>();
        eo->name = jObj["name"].string_value();
        eo->active = jObj["active"].bool_value();
        eo->hideInHierarchy = jObj["hideInHierarchy"].bool_value();

        auto &components = jObj["components"].array_items();
        for (const auto &componentJObj : components)
        {
            auto &type = componentJObj["type"].string_value();
            auto component = Common::TypeJson::From(type, componentJObj);
            if (component != nullptr)
                context->AddComponent(eo, type, component);
        }

        return eo;
    }

    static std::string EOToJson(Context *context, std::shared_ptr<EngineObject> eo)
    {
        json11::Json::array componentJObjArr = {};

        auto &componentMap = eo->componentMap;
        for (const auto &kv : componentMap)
        {
            auto type = kv.first;
            auto &component = kv.second;
            auto componentJObj = Common::TypeJson::To(type, component);
            componentJObjArr.push_back(componentJObj);
        }

        json11::Json::object eoJObj = {
            {"name", eo->name},
            {"active", eo->active},
            {"hideInHierarchy", eo->hideInHierarchy},
            {"components", componentJObjArr}};

        auto j = json11::Json(eoJObj);
        return j.dump();
    }
}