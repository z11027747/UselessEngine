
#include "editor/json/component_json.h"
#include "render/material/material_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<Render::Material> ComponentJson<Render::Material>::From(Context *context, const json11::Json &j)
    {
        auto &imageNameJArr = j["imageNames"].array_items();

        auto material = std::make_shared<Render::Material>();
        material->pipelineName = j["pipelineName"].string_value();
        material->imageNames.clear();
        for (const auto &imageNameJObj : imageNameJArr)
        {
            material->imageNames.push_back(imageNameJObj.string_value());
        }
        material->castShadow = j["castShadow"].bool_value();

        return material;
    }

    template <>
    json11::Json ComponentJson<Render::Material>::To(Context *context,
                                                     std::shared_ptr<Render::Material> material)
    {
        auto &pipelineName = material->pipelineName;
        auto &imageNames = material->imageNames;
        auto isImageCube = material->isImageCube;
        auto castShadow = material->castShadow;

        auto jObj = json11::Json::object{
            {"type", Type_Render_Material},
            {"pipelineName", pipelineName},
            {"imageNames", imageNames},
            {"isImageCube", isImageCube},
            {"castShadow", castShadow}};

        return jObj;
    }
}