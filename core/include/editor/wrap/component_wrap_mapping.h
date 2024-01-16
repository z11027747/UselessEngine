#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "logic/transform/transform_comp.h"
#include "logic/camera/camera_comp.h"

class Context;

namespace Editor
{
    template <typename T>
    static void ComponentDraw(Context *context, std::shared_ptr<void> component, bool isFirst)
    {
        ComponentWrap<T>::Draw(context, std::static_pointer_cast<T>(component), isFirst);
    }

    static std::unordered_map<std::type_index, std::function<void(Context *, std::shared_ptr<void>, bool)>> drawFuncMap{
        {typeid(Logic::Transform), ComponentDraw<Logic::Transform>},
        {typeid(Logic::Camera), ComponentDraw<Logic::Camera>},
        {typeid(Render::DirectionLight), ComponentDraw<Render::DirectionLight>},
        {typeid(Render::Mesh), ComponentDraw<Render::Mesh>},
        {typeid(Render::Material), ComponentDraw<Render::Material>}};

    static bool HasWrap(Context *context, std::type_index typeId)
    {
        auto it = drawFuncMap.find(typeId);
        if (it == drawFuncMap.end())
            return false;

        return true;
    }

    static void DrawWrap(Context *context,
                         std::type_index typeId, std::shared_ptr<void> component, bool isFirst)
    {
        auto it = drawFuncMap.find(typeId);
        if (it == drawFuncMap.end())
            return;

        drawFuncMap[typeId](context, component, isFirst);
    }
}
