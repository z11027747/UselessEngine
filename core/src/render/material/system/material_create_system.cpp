#pragma once

#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/material/material_comp.h"
#include "render/material/material_system.h"
#include "render/material/material_logic.h"
#include "render/material/material_json.h"
#include "define.hpp"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(Material);
        REGIST_ADD_REMOVE(Material, MaterialLogic);
        REGIST_JSON(Material, MaterialJson);

        MaterialGlobalDescriptorLogic::Create(context);

        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();
        auto &passMap = global->passMap;

        auto shadowPass = passMap[Define::Pass::Shadow];
        PipelineLogic::Create(context, Define::Pipeline::Shadow, shadowPass);

        auto forwardPass = passMap[Define::Pass::Forward];
        PipelineLogic::Create(context, Define::Pipeline::Skybox, forwardPass);
        PipelineLogic::Create(context, Define::Pipeline::LightModel, forwardPass);
        PipelineLogic::Create(context, Define::Pipeline::Color, forwardPass);
        PipelineLogic::Create(context, Define::Pipeline::Dissolve, forwardPass);
        PipelineLogic::Create(context, Define::Pipeline::Water, forwardPass);

        auto deferredPass = passMap[Define::Pass::Deferred];
        PipelineLogic::Create(context, Define::Pipeline::Deferred_LightModel, deferredPass);
        PipelineLogic::Create(context, Define::Pipeline::Deferred_Shading, deferredPass);
        PipelineLogic::Create(context, Define::Pipeline::Deferred_Volumn, deferredPass);
        PipelineLogic::Create(context, Define::Pipeline::Deferred_PointLight, deferredPass);

        auto postProcessPass = passMap[Define::Pass::PostProcess];
        PipelineLogic::Create(context, Define::Pipeline::PostProcess_SSAO, postProcessPass);
        PipelineLogic::Create(context, Define::Pipeline::PostProcess_ToonMapping, postProcessPass);
        PipelineLogic::Create(context, Define::Pipeline::PostProcess_GaussBlur, postProcessPass);
        PipelineLogic::Create(context, Define::Pipeline::PostProcess_Bloom, postProcessPass);
        PipelineLogic::Create(context, Define::Pipeline::PostProcess_Global, postProcessPass);

        MaterialInstanceLogic::CreateCache(context);

        auto instanceCache = globalEO->GetComponent<MaterialInstanceCache>();

        // post-process global instance
        auto postProcessSSAOInstance = MaterialInstanceLogic::Create(context, Define::Pipeline::PostProcess_SSAO);
        auto postProcessToonMappingInstance = MaterialInstanceLogic::Create(context, Define::Pipeline::PostProcess_ToonMapping);
        auto postProcessGaussBlurInstance = MaterialInstanceLogic::Create(context, Define::Pipeline::PostProcess_GaussBlur);
        auto postProcessBloomInstance = MaterialInstanceLogic::Create(context, Define::Pipeline::PostProcess_Bloom);
        auto postProcessGlobalInstance = MaterialInstanceLogic::Create(context, Define::Pipeline::PostProcess_Global);

        instanceCache->globalInstanceMap[Define::Pipeline::PostProcess_SSAO] = postProcessSSAOInstance;
        instanceCache->globalInstanceMap[Define::Pipeline::PostProcess_ToonMapping] = postProcessToonMappingInstance;
        instanceCache->globalInstanceMap[Define::Pipeline::PostProcess_GaussBlur] = postProcessGaussBlurInstance;
        instanceCache->globalInstanceMap[Define::Pipeline::PostProcess_Bloom] = postProcessBloomInstance;
        instanceCache->globalInstanceMap[Define::Pipeline::PostProcess_Global] = postProcessGlobalInstance;

        // deferred global instance
        auto deferredShadingInstance = MaterialInstanceLogic::Create(context, Define::Pipeline::Deferred_Shading);
        instanceCache->globalInstanceMap[Define::Pass::Deferred] = deferredShadingInstance;
    }
}