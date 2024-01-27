#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
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
#define TYPE_TO_CONSTRUCT(class)                                 \
    {                                                            \
        class ::type, []() { return std::make_shared<class>(); } \
    }

    class TypeConstruct final
    {
        inline static std::unordered_map<std::string, std::function<std::shared_ptr<void>()>> map{
            // logic
            TYPE_TO_CONSTRUCT(Logic::Camera),
            TYPE_TO_CONSTRUCT(Logic::HitRay),
            TYPE_TO_CONSTRUCT(Logic::MoveFowrard),
            TYPE_TO_CONSTRUCT(Logic::MoveFollow),
            TYPE_TO_CONSTRUCT(Logic::RotateAround),
            TYPE_TO_CONSTRUCT(Logic::Transform),
            // render
            TYPE_TO_CONSTRUCT(Render::DirectionLight),
            TYPE_TO_CONSTRUCT(Render::PointLight),
            TYPE_TO_CONSTRUCT(Render::SpotLight),
            TYPE_TO_CONSTRUCT(Render::Material),
            TYPE_TO_CONSTRUCT(Render::Mesh),
        };

    public:
        static std::shared_ptr<void> Create(const std::string &type)
        {
            return map[type]();
        }
    };
}