
#include "render/post_process/post_process_comp.h"
#include "render/post_process/post_process_logic.h"
#include "context.hpp"

namespace Render
{
    std::shared_ptr<void> PostProcessJson::Parse(const json11::Json &j)
    {
        auto jObj = j.object_items();

        auto &toonMappingParamsJArr = jObj["toonMappingParams"].array_items();
        auto toonMappingParamsX = (float)toonMappingParamsJArr.at(0).number_value();
        auto toonMappingParamsY = (float)toonMappingParamsJArr.at(1).number_value();
        auto toonMappingParamsZ = (float)toonMappingParamsJArr.at(2).number_value();
        auto toonMappingParamsW = (float)toonMappingParamsJArr.at(3).number_value();

        auto &gaussBlurParamsJArr = jObj["gaussBlurParams"].array_items();
        auto gaussBlurParamsX = (float)gaussBlurParamsJArr.at(0).number_value();
        auto gaussBlurParamsY = (float)gaussBlurParamsJArr.at(1).number_value();
        auto gaussBlurParamsZ = (float)gaussBlurParamsJArr.at(2).number_value();
        auto gaussBlurParamsW = (float)gaussBlurParamsJArr.at(3).number_value();

        auto &bloomParamsJArr = jObj["bloomParams"].array_items();
        auto bloomParamsX = (float)bloomParamsJArr.at(0).number_value();
        auto bloomParamsY = (float)bloomParamsJArr.at(1).number_value();
        auto bloomParamsZ = (float)bloomParamsJArr.at(2).number_value();
        auto bloomParamsW = (float)bloomParamsJArr.at(3).number_value();

        auto postProcess = std::make_shared<Render::PostProcess>();
        postProcess->toonMappingParams = glm::vec4(toonMappingParamsX, toonMappingParamsY, toonMappingParamsZ, toonMappingParamsW);
        postProcess->gaussBlurParams = glm::vec4(gaussBlurParamsX, gaussBlurParamsY, gaussBlurParamsZ, gaussBlurParamsW);
        postProcess->bloomParams = glm::vec4(bloomParamsX, bloomParamsY, bloomParamsZ, bloomParamsW);

        return postProcess;
    }

    json11::Json PostProcessJson::Serialize(std::shared_ptr<void> component)
    {
        auto postProcess = std::static_pointer_cast<Render::PostProcess>(component);

        auto &toonMappingParams = postProcess->toonMappingParams;
        auto &gaussBlurParams = postProcess->gaussBlurParams;
        auto &bloomParams = postProcess->bloomParams;

        auto toonMappingParamsJArr = json11::Json::array{toonMappingParams.x, toonMappingParams.y, toonMappingParams.z, toonMappingParams.w};
        auto gaussBlurParamsJArr = json11::Json::array{gaussBlurParams.x, gaussBlurParams.y, gaussBlurParams.z, gaussBlurParams.w};
        auto bloomParamsJArr = json11::Json::array{bloomParams.x, bloomParams.y, bloomParams.z, bloomParams.w};

        auto jObj = json11::Json::object{
            {"type", Render::PostProcess::type},
            {"toonMappingParams", toonMappingParamsJArr},
            {"gaussBlurParams", gaussBlurParamsJArr},
            {"bloomParams", bloomParamsJArr}};

        auto j = json11::Json(jObj);
        return j;
    }
}