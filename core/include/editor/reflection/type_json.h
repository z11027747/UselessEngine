#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <json/json11.hpp>
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/hit/hit_comp.h"
#include "logic/move/move_comp.h"
#include "logic/rotate/rotate_comp.h"
#include "logic/transform/transform_comp.h"

namespace Editor
{
#define TYPE_JSON_FROM(class)                     \
    {                                             \
        class ::type, [&](const json11::Json &j)  \
        { return ComponentJson<class>::From(j); } \
    }

#define TYPE_JSON_TO(class)                                \
    {                                                      \
        class ::type, [&](std::shared_ptr<void> component) \
        { return ComponentJson<class>::To(component); }    \
    }

    class TypeJson final
    {
        inline static std::unordered_map<std::string, std::function<std::shared_ptr<void>(const json11::Json &)>>
            fromMap{
                // logic
                TYPE_JSON_FROM(Logic::Camera),
                // TYPE_JSON_FROM(Logic::HitRay),
                // TYPE_JSON_FROM(Logic::MoveFowrard),
                // TYPE_JSON_FROM(Logic::MoveFollow),
                // TYPE_JSON_FROM(Logic::RotateAround),
                TYPE_JSON_FROM(Logic::Transform),
                // render
                TYPE_JSON_FROM(Render::DirectionLight),
                TYPE_JSON_FROM(Render::PointLight),
                // TYPE_JSON_FROM(Render::SpotLight),
                TYPE_JSON_FROM(Render::Material),
                TYPE_JSON_FROM(Render::Mesh),
            };

        inline static std::unordered_map<std::string, std::function<json11::Json(std::shared_ptr<void>)>>
            toMap{
                // logic
                TYPE_JSON_TO(Logic::Camera),
                // TYPE_JSON_TO(Logic::HitRay),
                // TYPE_JSON_TO(Logic::MoveFowrard),
                // TYPE_JSON_TO(Logic::MoveFollow),
                // TYPE_JSON_TO(Logic::RotateAround),
                TYPE_JSON_TO(Logic::Transform),
                // render
                TYPE_JSON_TO(Render::DirectionLight),
                TYPE_JSON_TO(Render::PointLight),
                // TYPE_JSON_TO(Render::SpotLight),
                TYPE_JSON_TO(Render::Material),
                TYPE_JSON_TO(Render::Mesh),
            };

    public:
        static std::shared_ptr<void> From(const std::string &type, const json11::Json &j)
        {
            auto it = fromMap.find(type);
            if (it == fromMap.end())
                return nullptr;

            return fromMap[type](j);
        }

        static json11::Json To(const std::string &type, std::shared_ptr<void> component)
        {
            auto it = toMap.find(type);
            if (it == toMap.end())
                return {};

            return toMap[type](component);
        }
    };
}