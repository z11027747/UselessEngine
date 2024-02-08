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
        auto forwardPass = passMap[Define::Pass::Forward];
        auto deferredPass = passMap[Define::Pass::Deferred];
        auto postProcessPass = passMap[Define::Pass::PostProcess];

        PipelineLogic::Create(context, Define::Pipeline::Shadow, shadowPass);
        PipelineLogic::Create(context, Define::Pipeline::Skybox, forwardPass);
        PipelineLogic::Create(context, Define::Pipeline::LightModel, forwardPass);
        PipelineLogic::Create(context, Define::Pipeline::Color, forwardPass);
        PipelineLogic::Create(context, Define::Pipeline::Deferred_LightModel, deferredPass);
        PipelineLogic::Create(context, Define::Pipeline::Deferred_Shading, deferredPass);
        PipelineLogic::Create(context, Define::Pipeline::Deferred_Volumn, deferredPass);
        PipelineLogic::Create(context, Define::Pipeline::Deferred_PointLight, deferredPass);
        PipelineLogic::Create(context, Define::Pipeline::PostProcess_Bloom, postProcessPass);
        PipelineLogic::Create(context, Define::Pipeline::PostProcess_ToonMapping, postProcessPass);
        PipelineLogic::Create(context, Define::Pipeline::PostProcess_Global, postProcessPass);

        MaterialInstanceLogic::CreateCache(context);

        auto instanceCache = globalEO->GetComponent<MaterialInstanceCache>();

        auto postProcessMaterialInfo = std::make_shared<MaterialInfo>();
        postProcessMaterialInfo->pipelineName = Define::Pipeline::PostProcess_Global;
        instanceCache->globalInstanceMap[Define::Pass::PostProcess] =
            MaterialInstanceLogic::Create(context, postProcessMaterialInfo);

        auto deferredMaterialInfo = std::make_shared<MaterialInfo>();
        deferredMaterialInfo->pipelineName = Define::Pipeline::Deferred_Shading;
        instanceCache->globalInstanceMap[Define::Pass::Deferred] =
            MaterialInstanceLogic::Create(context, deferredMaterialInfo);
    }
}