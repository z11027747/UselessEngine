#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <json/json11.hpp>
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/unit/unit_comp.h"
#include "logic/transform/transform_comp.h"
#include "logic/camera/camera_comp.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
    template <typename T>
    static void ComponentFromJson(Context *context,
                                  std::shared_ptr<EngineObject> eo, const json11::Json &j)
    {
        auto component = ComponentJson<T>::From(context, j);
        eo->AddComponent<T>(component);
    }

    static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<EngineObject>, const json11::Json &)>> fromJsonFuncMap{
        {"Logic::Camera", ComponentFromJson<Logic::Camera>},
        {"Logic::Transform", ComponentFromJson<Logic::Transform>},
        {"Render::DirectionLight", ComponentFromJson<Render::DirectionLight>},
        {"Render::Material", ComponentFromJson<Render::Material>},
        {"Render::Mesh", ComponentFromJson<Render::Mesh>},
        {"Render::Unit", ComponentFromJson<Render::Unit>}};

    static std::shared_ptr<EngineObject> EOFromJson(Context *context, const json11::Json &jObj)
    {
        auto eo = std::make_shared<EngineObject>();
        eo->name = jObj["name"].string_value();
        eo->active = jObj["active"].bool_value();
        eo->hideInHierarchy = jObj["hideInHierarchy"].bool_value();

        auto &components = jObj["components"].array_items();
        for (const auto &component : components)
        {
            auto &componentType = component["type"].string_value();

            auto it = fromJsonFuncMap.find(componentType);
            if (it == fromJsonFuncMap.end())
                continue;

            fromJsonFuncMap[componentType](context, eo, component);
        }

        return eo;
    }

    template <typename T>
    static json11::Json ComponentToJson(Context *context, std::shared_ptr<void> component)
    {
        return ComponentJson<T>::To(context, std::static_pointer_cast<T>(component));
    }

    static std::unordered_map<std::type_index, std::function<json11::Json(Context *, std::shared_ptr<void>)>> toJsonFuncMap{
        {typeid(Logic::Camera), ComponentToJson<Logic::Camera>},
        {typeid(Logic::Transform), ComponentToJson<Logic::Transform>},
        {typeid(Render::DirectionLight), ComponentToJson<Render::DirectionLight>},
        {typeid(Render::Material), ComponentToJson<Render::Material>},
        {typeid(Render::Mesh), ComponentToJson<Render::Mesh>},
        {typeid(Render::Unit), ComponentToJson<Render::Unit>}};

    static std::string EOToJson(Context *context, std::shared_ptr<EngineObject> eo)
    {
        json11::Json::array componentJObjArr = {};
        auto &componentMap = eo->componentMap;
        for (const auto &kv : componentMap)
        {
            auto typeId = kv.first;
            auto &component = kv.second;

            auto it = toJsonFuncMap.find(typeId);
            if (it == toJsonFuncMap.end())
                continue;

            auto componentJObj = toJsonFuncMap[typeId](context, component);
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