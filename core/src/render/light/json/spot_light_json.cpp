
#include "render/light/light_comp.h"
#include "render/light/light_json.h"
#include "context.hpp"

namespace Render
{
    std::shared_ptr<void> SpotLightJson::Parse(const json11::Json &j)
    {
        auto jObj = j.object_items();

        auto &colorJArr = jObj["color"].array_items();
        auto colorR = (float)colorJArr.at(0).number_value();
        auto colorG = (float)colorJArr.at(1).number_value();
        auto colorB = (float)colorJArr.at(2).number_value();

        auto &cutOffJArr = jObj["cutOff"].array_items();
        auto cutOffX = (float)cutOffJArr.at(0).number_value();
        auto cutOffY = (float)cutOffJArr.at(1).number_value();
        auto cutOffZ = (float)cutOffJArr.at(2).number_value();
        auto cutOffW = (float)cutOffJArr.at(3).number_value();

        auto spotLight = std::make_shared<Render::SpotLight>();
        spotLight->color = glm::vec3(colorR, colorG, colorB);
        spotLight->cutOff = glm::vec4(cutOffX, cutOffY, cutOffZ, cutOffW);
        spotLight->hasShadow = jObj["hasShadow"].bool_value();

        return spotLight;
    }

    json11::Json SpotLightJson::Serialize(std::shared_ptr<void> component)
    {
        auto spotLight = std::static_pointer_cast<Render::SpotLight>(component);

        auto &color = spotLight->color;
        auto colorJArr = json11::Json::array{color.r, color.g, color.b};

        auto &cutOff = spotLight->cutOff;
        auto cutOffJArr = json11::Json::array{cutOff.x, cutOff.y, cutOff.z, cutOff.w};

        auto jObj = json11::Json::object{
            {"type", Render::SpotLight::type},
            {"color", colorJArr},
            {"cutOff", cutOffJArr},
            {"hasShadow", spotLight->hasShadow}};

        auto j = json11::Json(jObj);
        return j;
    }
}