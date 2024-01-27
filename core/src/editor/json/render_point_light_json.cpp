
#include "editor/json/component_json.h"
#include "render/light/light_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<void> ComponentJson<Render::PointLight>::From(const json11::Json &j)
    {
        auto jObj = j.object_items();

        auto &colorJArr = jObj["color"].array_items();
        auto colorR = (float)colorJArr.at(0).number_value();
        auto colorG = (float)colorJArr.at(1).number_value();
        auto colorB = (float)colorJArr.at(2).number_value();

        auto &clqJArr = jObj["clq"].array_items();
        auto clqX = (float)clqJArr.at(0).number_value();
        auto clqY = (float)clqJArr.at(1).number_value();
        auto clqZ = (float)clqJArr.at(2).number_value();

        auto pointLight = std::make_shared<Render::PointLight>();
        pointLight->color = glm::vec3(colorR, colorG, colorB);
        pointLight->clq = glm::vec3(clqX, clqY, clqZ);
        pointLight->hasShadow = jObj["hasShadow"].bool_value();

        return pointLight;
    }

    template <>
    json11::Json ComponentJson<Render::PointLight>::To(std::shared_ptr<void> component)
    {
        auto pointLight = std::static_pointer_cast<Render::PointLight>(component);

        auto &color = pointLight->color;
        auto colorJArr = json11::Json::array{color.r, color.g, color.b};

        auto &clq = pointLight->clq;
        auto clqJArr = json11::Json::array{clq.x, clq.y, clq.z};

        auto jObj = json11::Json::object{
            {"type", Render::PointLight::type},
            {"color", colorJArr},
            {"clq", clqJArr},
            {"hasShadow", pointLight->hasShadow}};

        return jObj;
    }
}