#pragma once

#include <unordered_map>
#include <string>
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
    static void ComponentDraw(Context *context, std::shared_ptr<void> component, bool isInit)
    {
        ComponentWrap<T>::Draw(context, std::static_pointer_cast<T>(component), isInit);
    }

    static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<void>, bool)>>
        drawMap{
            {Logic::Transform::type, ComponentDraw<Logic::Transform>},
            {Logic::Camera::type, ComponentDraw<Logic::Camera>},
            {Render::DirectionLight::type, ComponentDraw<Render::DirectionLight>},
            {Render::PointLight::type, ComponentDraw<Render::PointLight>},
            // Render::SpotLight::type, ComponentDraw<Render::SpotLight>},
            {Render::Mesh::type, ComponentDraw<Render::Mesh>},
            {Render::Material::type, ComponentDraw<Render::Material>}};

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
