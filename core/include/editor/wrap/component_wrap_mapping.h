#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "logic/transform/transform_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/rotate/rotate_comp.h"

class Context;

namespace Editor
{
    template <typename T>
    static void ComponentDraw(Context *context, std::shared_ptr<void> component, bool isInit)
    {
        ComponentWrap<T>::Draw(context, std::static_pointer_cast<T>(component), isInit);
    }

#define TYPE_TO_DRAW(class)                \
    {                                      \
        class ::type, ComponentDraw<class> \
    }

    static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<void>, bool)>>
        drawMap{
            // logic
            TYPE_TO_DRAW(Logic::Transform),
            TYPE_TO_DRAW(Logic::Camera),
            TYPE_TO_DRAW(Logic::RotateAround),
            // render
            TYPE_TO_DRAW(Render::DirectionLight),
            TYPE_TO_DRAW(Render::PointLight),
            // TYPE_TO_DRAW(Render::SpotLight),
            TYPE_TO_DRAW(Render::Mesh),
            TYPE_TO_DRAW(Render::Material),
        };

    static bool HasWrap(Context *context, std::string type)
    {
        auto it = drawMap.find(type);
        if (it == drawMap.end())
            return false;

        return true;
    }

    static void DrawWrap(Context *context,
                         std::string type, std::shared_ptr<void> component, bool isInit)
    {
        if (HasWrap(context, type))
            drawMap[type](context, component, isInit);
    }
}
