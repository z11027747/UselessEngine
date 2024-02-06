
#include <unordered_map>
#include <functional>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/material/material_logic.h"
#include "render/material/impl/material_light_model_logic.h"
#include "render/material/impl/material_color_logic.h"
#include "render/material/impl/material_shadow_logic.h"
#include "render/material/impl/material_skybox_logic.h"
#include "render/material/impl/material_deferred_light_model_logic.h"
#include "render/material/impl/material_deferred_shading_logic.h"
#include "render/material/impl/material_deferred_volumn_logic.h"
#include "render/material/impl/material_deferred_point_light_logic.h"
#include "render/material/impl/material_post_process_logic.h"
#include "context.h"

namespace Render
{
    void MaterialPipelineLogic::SetVertexAttrDescriptions(Context *context,
                                                          std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<GraphicsPipeline>)>>
            funcMap{
                {Define::Pipeline::Color, MaterialColorPipelineLogic::SetVertexAttrDescriptions},
                {Define::Pipeline::LightModel, MaterialLightModelPipelineLogic::SetVertexAttrDescriptions},
                {Define::Pipeline::Deferred_LightModel, MaterialDeferredLightModelPipelineLogic::SetVertexAttrDescriptions},
                {Define::Pipeline::Deferred_Volumn, MaterialDeferredVolumnPipelineLogic::SetVertexAttrDescriptions},
                {Define::Pipeline::Shadow, MaterialShadowPipelineLogic::SetVertexAttrDescriptions},
                {Define::Pipeline::Skybox, MaterialSkyboxPipelineLogic::SetVertexAttrDescriptions},
            };

        auto &name = graphicsPipeline->name;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, graphicsPipeline);
    }
    void MaterialPipelineLogic::SetViewport(Context *context,
                                            std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<GraphicsPipeline>)>>
            funcMap{
                {Define::Pipeline::Shadow, MaterialShadowPipelineLogic::SetViewport},
                {Define::Pipeline::PostProcess_Bloom, MaterialPostProcessPipelineLogic::SetViewport},
                {Define::Pipeline::PostProcess_ToonMapping, MaterialPostProcessPipelineLogic::SetViewport},
                {Define::Pipeline::PostProcess_Global, MaterialPostProcessPipelineLogic::SetViewport},
            };

        auto &name = graphicsPipeline->name;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, graphicsPipeline);
    }
    void MaterialPipelineLogic::SetRasterizationCreateInfo(Context *context,
                                                           std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<GraphicsPipeline>)>>
            funcMap{
                {Define::Pipeline::Color, MaterialColorPipelineLogic::SetRasterizationCreateInfo},
                {Define::Pipeline::LightModel, MaterialLightModelPipelineLogic::SetRasterizationCreateInfo},
                {Define::Pipeline::Deferred_LightModel, MaterialDeferredLightModelPipelineLogic::SetRasterizationCreateInfo},
                {Define::Pipeline::Deferred_Shading, MaterialDeferredShadingPipelineLogic::SetRasterizationCreateInfo},
                {Define::Pipeline::Deferred_Volumn, MaterialDeferredVolumnPipelineLogic::SetRasterizationCreateInfo},
                {Define::Pipeline::Deferred_PointLight, MaterialDeferredPointLightPipelineLogic::SetRasterizationCreateInfo},
                {Define::Pipeline::Shadow, MaterialShadowPipelineLogic::SetRasterizationCreateInfo},
                {Define::Pipeline::Skybox, MaterialSkyboxPipelineLogic::SetRasterizationCreateInfo},
            };

        auto &name = graphicsPipeline->name;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, graphicsPipeline);
    }
    void MaterialPipelineLogic::SetMultisampleCreateInfo(Context *context,
                                                         std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<GraphicsPipeline>)>>
            funcMap{
                {Define::Pipeline::LightModel, MaterialLightModelPipelineLogic::SetMultisampleCreateInfo},
                {Define::Pipeline::Deferred_LightModel, MaterialDeferredLightModelPipelineLogic::SetMultisampleCreateInfo},
            };

        auto &name = graphicsPipeline->name;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, graphicsPipeline);
    }
    void MaterialPipelineLogic::SetDepthStencilCreateInfo(Context *context,
                                                          std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<GraphicsPipeline>)>>
            funcMap{
                {Define::Pipeline::Color, MaterialColorPipelineLogic::SetDepthStencilCreateInfo},
                {Define::Pipeline::Skybox, MaterialSkyboxPipelineLogic::SetDepthStencilCreateInfo},
                {Define::Pipeline::Deferred_LightModel, MaterialDeferredLightModelPipelineLogic::SetDepthStencilCreateInfo},
                {Define::Pipeline::Deferred_Shading, MaterialDeferredShadingPipelineLogic::SetDepthStencilCreateInfo},
                {Define::Pipeline::Deferred_Volumn, MaterialDeferredVolumnPipelineLogic::SetDepthStencilCreateInfo},
                {Define::Pipeline::Deferred_PointLight, MaterialDeferredPointLightPipelineLogic::SetDepthStencilCreateInfo},
            };

        auto &name = graphicsPipeline->name;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, graphicsPipeline);
    }
    void MaterialPipelineLogic::SetColorBlendStage(Context *context,
                                                   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<GraphicsPipeline>)>>
            funcMap{
                {Define::Pipeline::Color, MaterialColorPipelineLogic::SetColorBlendStage},
                {Define::Pipeline::Deferred_LightModel, MaterialDeferredLightModelPipelineLogic::SetColorBlendStage},
                {Define::Pipeline::Deferred_Volumn, MaterialDeferredVolumnPipelineLogic::SetColorBlendStage},
            };

        auto &name = graphicsPipeline->name;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, graphicsPipeline);
    }
}