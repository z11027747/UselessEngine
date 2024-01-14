
#include "editor/json/component_json.h"
#include "render/light/light_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    json11::Json ComponentJson<Render::DirectionLight>::To(Context *context,
                                                           std::shared_ptr<Render::DirectionLight> directionLight)
    {
        auto &color = directionLight->color;
        auto &params = directionLight->params;
        auto hasShadow = directionLight->hasShadow;

        auto jObj = json11::Json::object{{"color", json11::Json::array{color.x, color.y, color.z}},
                                         {"params", json11::Json::array{params.x, params.y, params.z, params.w}},
                                         {"hasShadow", hasShadow}};

        return json11::Json::object{{"directionLight", jObj}};
    }
}