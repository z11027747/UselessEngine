
#include "editor/json/component_json.h"
#include "render/material/material_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<Render::Material> ComponentJson<Render::Material>::From(Context *context, const json11::Json &j)
    {
        auto material = std::make_shared<Render::Material>();
        material->pipelineName = j["pipelineName"].string_value();
        material->image0Name = j["image0Name"].string_value();
        material->castShadow = j["castShadow"].bool_value();

        return material;
    }

    template <>
    json11::Json ComponentJson<Render::Material>::To(Context *context,
                                                     std::shared_ptr<Render::Material> material)

    {
        auto &pipelineName = material->pipelineName;
        auto &image0Name = material->image0Name;
        auto castShadow = material->castShadow;

        auto jObj = json11::Json::object{
            {"type", "material"},
            {"pipelineName", pipelineName},
            {"image0Name", image0Name},
            {"castShadow", castShadow}};

        return jObj;
    }
}