
#include "render/light/light_comp.h"
#include "render/light/light_json.h"
#include "context.hpp"

namespace Render
{
    std::shared_ptr<void> DirectionLightJson::Parse(const json11::Json &j)
    {
        auto jObj = j.object_items();

        auto &colorJArr = jObj["color"].array_items();
        auto colorR = (float)colorJArr.at(0).number_value();
        auto colorG = (float)colorJArr.at(1).number_value();
        auto colorB = (float)colorJArr.at(2).number_value();

        auto directionLight = std::make_shared<Render::DirectionLight>();
        directionLight->color = glm::vec3(colorR, colorG, colorB);
        directionLight->hasShadow = jObj["hasShadow"].bool_value();

        return directionLight;
    }

    json11::Json DirectionLightJson::Serialize(std::shared_ptr<void> component)
    {
        auto directionLight = std::static_pointer_cast<Render::DirectionLight>(component);

        auto &color = directionLight->color;
        auto hasShadow = directionLight->hasShadow;

        auto colorJArr = json11::Json::array{color.x, color.y, color.z};

        auto jObj = json11::Json::object{
            {"type", Render::DirectionLight::type},
            {"color", colorJArr},
            {"hasShadow", hasShadow}};

        auto j = json11::Json(jObj);
        return j;
    }
}