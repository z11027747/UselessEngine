#pragma once

#include <string>
#include <memory>
#include <json/json11.hpp>

class Context;

namespace Editor
{
    //logic::
    const std::string Type_Logic_Camera = "Logic::Camera";
    const std::string Type_Logic_Transform = "Logic::Transform";
    const std::string Type_Logic_MoveFowrard = "Logic::MoveFowrard";
    const std::string Type_Logic_MoveFollow = "Logic::MoveFollow";
    const std::string Type_Logic_RotateAround = "Logic::RotateAround";
    //render::
    const std::string Type_Render_DirectionLight = "Render::DirectionLight";
    const std::string Type_Render_PointLight = "Render::PointLight";
    const std::string Type_Render_SpotLight = "Render::SpotLight";
    const std::string Type_Render_Material = "Render::Material";
    const std::string Type_Render_Mesh = "Render::Mesh";
    const std::string Type_Render_Unit = "Render::Unit";

    template <typename T>
    class ComponentJson
    {
    public:
        static std::shared_ptr<T> From(Context *, const json11::Json &);
        static json11::Json To(Context *, std::shared_ptr<T>);
    };
}