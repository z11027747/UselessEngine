
#include "editor/json/component_json.h"
#include "render/material/material_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<void> ComponentJson<Render::Material>::From(const json11::Json &j)
    {
        auto jObj = j.object_items();

        auto &imageNameJArr = jObj["imageNames"].array_items();
        auto &paramsJArr = jObj["params"].array_items();

        auto material = std::make_shared<Render::Material>();
        auto materialInfo = std::make_shared<Render::MaterialInfo>();
        materialInfo->pipelineName = jObj["pipelineName"].string_value();
        materialInfo->imageNames.clear();
        for (const auto &imageNameJObj : imageNameJArr)
        {
            materialInfo->imageNames.push_back(imageNameJObj.string_value());
        }
        materialInfo->params.clear();
        for (const auto &paramsJObj : paramsJArr)
        {
            auto &paramsJObjArr = paramsJObj.array_items();
            auto paramsX = (float)paramsJObjArr.at(0).number_value();
            auto paramsY = (float)paramsJObjArr.at(1).number_value();
            auto paramsZ = (float)paramsJObjArr.at(2).number_value();
            auto paramsW = (float)paramsJObjArr.at(3).number_value();
            materialInfo->params.push_back(glm::vec4(paramsX, paramsY, paramsZ, paramsW));
        }
        materialInfo->isImageCube = jObj["isImageCube"].bool_value();
        materialInfo->castShadow = jObj["castShadow"].bool_value();
        materialInfo->renderQueue = jObj["renderQueue"].int_value();
        material->info = materialInfo;

        return material;
    }

    template <>
    json11::Json ComponentJson<Render::Material>::To(std::shared_ptr<void> component)
    {
        auto material = std::static_pointer_cast<Render::Material>(component);

        auto &materialInfo = material->info;

        auto &pipelineName = materialInfo->pipelineName;
        auto &imageNames = materialInfo->imageNames;
        auto &params = materialInfo->params;
        auto isImageCube = materialInfo->isImageCube;
        auto castShadow = materialInfo->castShadow;
        auto renderQueue = materialInfo->renderQueue;

        auto paramsJArr = json11::Json::array{};
        for (auto &param : params)
        {
            auto paramsJObjArr = json11::Json::array{param.x, param.y, param.z, param.w};
            paramsJArr.push_back(paramsJObjArr);
        }

        auto jObj = json11::Json::object{
            {"type", Render::Material::type},
            {"pipelineName", pipelineName},
            {"imageNames", imageNames},
            {"params", paramsJArr},
            {"isImageCube", isImageCube},
            {"castShadow", castShadow},
            {"renderQueue", renderQueue}};

        auto j = json11::Json(jObj);
        return j;
    }
}