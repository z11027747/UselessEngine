
#include "editor/json/component_json.h"
#include "render/light/light_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<Render::PointLight> ComponentJson<Render::PointLight>::From(Context *context, const json11::Json &j)
    {
        auto &colorJArr = j["color"].array_items();
        auto colorR = (float)colorJArr.at(0).number_value();
        auto colorG = (float)colorJArr.at(1).number_value();
        auto colorB = (float)colorJArr.at(2).number_value();

        auto &clqJArr = j["clq"].array_items();
        auto clqX = (float)clqJArr.at(0).number_value();
        auto clqY = (float)clqJArr.at(1).number_value();
        auto clqZ = (float)clqJArr.at(2).number_value();

        auto pointLight = std::make_shared<Render::PointLight>();
        pointLight->color = glm::vec3(colorR, colorG, colorB);
        pointLight->clq = glm::vec3(clqX, clqY, clqZ);
        pointLight->hasShadow = j["hasShadow"].bool_value();

        return pointLight;
    }

    template <>
    json11::Json ComponentJson<Render::PointLight>::To(Context *context,
                                                       std::shared_ptr<Render::PointLight> pointLight)
    {
        auto &color = pointLight->color;
        auto colorJArr = json11::Json::array{color.r, color.g, color.b};

        auto &clq = pointLight->clq;
        auto clqJArr = json11::Json::array{clq.x, clq.y, clq.z};

        auto jObj = json11::Json::object{
            {"type", Type_Render_PointLight},
            {"color", colorJArr},
            {"clq", clqJArr},
            {"hasShadow", pointLight->hasShadow}};

        return jObj;
    }
}