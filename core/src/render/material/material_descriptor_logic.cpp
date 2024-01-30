
#include <unordered_map>
#include <functional>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/material/material_logic.h"
#include "render/material/impl/material_light_model_logic.h"
#include "render/material/impl/material_color_logic.h"
#include "render/material/impl/material_shadow_logic.h"
#include "render/material/impl/material_skybox_logic.h"
#include "render/material/impl/material_post_process_logic.h"
#include "context.h"

namespace Render
{
    void MaterialDescriptorLogic::CreateSetLayout(Context *context,
                                                  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<GraphicsPipeline>)>>
            funcMap{
                {Define::Pipeline::Color, MaterialColorDescriptorLogic::CreateSetLayout},
                {Define::Pipeline::LightModel, MaterialLightModelDescriptorLogic::CreateSetLayout},
                {Define::Pipeline::Skybox, MaterialSkyboxDescriptorLogic::CreateSetLayout},
                {Define::Pipeline::PostProcess_Bloom, MaterialPostProcessBloomDescriptorLogic::CreateSetLayout},
            };

        auto &name = graphicsPipeline->name;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, graphicsPipeline);
    }
    void MaterialDescriptorLogic::DestroySetLayout(Context *context,
                                                   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
        if (descriptorSetLayout != nullptr)
        {
            DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
        }
    }
    void MaterialDescriptorLogic::AllocateAndUpdate(Context *context,
                                                    std::shared_ptr<MaterialInstance> instance)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<MaterialInstance>)>>
            funcMap{
                {Define::Pipeline::Color, MaterialColorDescriptorLogic::AllocateAndUpdate},
                {Define::Pipeline::LightModel, MaterialLightModelDescriptorLogic::AllocateAndUpdate},
                {Define::Pipeline::Skybox, MaterialSkyboxDescriptorLogic::AllocateAndUpdate},
            };

        auto &name = instance->info->pipelineName;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, instance);
    }
    void MaterialDescriptorLogic::Destroy(Context *context,
                                          std::shared_ptr<MaterialInstance> instance)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<MaterialInstance>)>>
            funcMap{
                {Define::Pipeline::Color, MaterialColorDescriptorLogic::Destroy},
                {Define::Pipeline::LightModel, MaterialLightModelDescriptorLogic::Destroy},
                {Define::Pipeline::Skybox, MaterialSkyboxDescriptorLogic::Destroy},
            };

        auto &name = instance->info->pipelineName;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, instance);
    }
}