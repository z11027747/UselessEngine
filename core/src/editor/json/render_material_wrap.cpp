
#include "editor/json/component_json.h"
#include "render/material/material_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<Render::Material> ComponentJson<Render::Material>::From(Context *context, const json11::Json &j)
    {
        auto &image0NameJArr = j["image0Names"].array_items();

        auto material = std::make_shared<Render::Material>();
        material->pipelineName = j["pipelineName"].string_value();
        for (const auto &image0NameJObj : image0NameJArr)
        {
            material->image0Names.push_back(image0NameJObj.string_value());
        }
        material->castShadow = j["castShadow"].bool_value();

        return material;
    }

    template <>
    json11::Json ComponentJson<Render::Material>::To(Context *context,
                                                     std::shared_ptr<Render::Material> material)
    {
        auto &pipelineName = material->pipelineName;
        auto &image0Names = material->image0Names;
        auto castShadow = material->castShadow;

        auto jObj = json11::Json::object{
            {"type", "Render::Material"},
            {"pipelineName", pipelineName},
            {"image0Names", image0Names},
            {"castShadow", castShadow}};

        return jObj;
    }
}