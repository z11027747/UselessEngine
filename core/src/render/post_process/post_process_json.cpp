
#include "render/post_process/post_process_comp.h"
#include "common/reflection/type_json.h"
#include "context.h"

template <>
std::shared_ptr<void> Common::ComponentJson<Render::PostProcess>::From(const json11::Json &j)
{
    auto jObj = j.object_items();

    auto &toonMappingParamsJArr = jObj["toonMappingParams"].array_items();
    auto toonMappingParamsX = (float)toonMappingParamsJArr.at(0).number_value();
    auto toonMappingParamsY = (float)toonMappingParamsJArr.at(1).number_value();
    auto toonMappingParamsZ = (float)toonMappingParamsJArr.at(2).number_value();
    auto toonMappingParamsW = (float)toonMappingParamsJArr.at(3).number_value();

    auto &bloomParamsJArr = jObj["bloomParams"].array_items();
    auto bloomParamsX = (float)bloomParamsJArr.at(0).number_value();
    auto bloomParamsY = (float)bloomParamsJArr.at(1).number_value();
    auto bloomParamsZ = (float)bloomParamsJArr.at(2).number_value();
    auto bloomParamsW = (float)bloomParamsJArr.at(3).number_value();

    auto postProcess = std::make_shared<Render::PostProcess>();
    postProcess->toonMappingParams = glm::vec4(toonMappingParamsX, toonMappingParamsY, toonMappingParamsZ, toonMappingParamsW);
    postProcess->bloomParams = glm::vec4(bloomParamsX, bloomParamsY, bloomParamsZ, bloomParamsW);

    return postProcess;
}

template <>
json11::Json Common::ComponentJson<Render::PostProcess>::To(std::shared_ptr<void> component)
{
    auto postProcess = std::static_pointer_cast<Render::PostProcess>(component);

    auto &toonMappingParams = postProcess->toonMappingParams;
    auto toonMappingParamsJArr = json11::Json::array{toonMappingParams.x, toonMappingParams.y, toonMappingParams.z, toonMappingParams.w};
    auto &bloomParams = postProcess->bloomParams;
    auto bloomParamsJArr = json11::Json::array{bloomParams.x, bloomParams.y, bloomParams.z, bloomParams.w};

    auto jObj = json11::Json::object{
        {"type", Render::PostProcess::type},
        {"toonMappingParams", toonMappingParamsJArr},
        {"bloomParams", bloomParamsJArr}};

    auto j = json11::Json(jObj);
    return j;
}