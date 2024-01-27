
#include <unordered_map>
#include <functional>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/material/material_logic.h"
#include "render/material/impl/material_light_model_logic.h"
#include "render/material/impl/material_color_logic.h"
#include "render/material/impl/material_shadow_logic.h"
#include "render/material/impl/material_skybox_logic.h"
#include "context.h"

namespace Render
{
    void MaterialDescriptorLogic::CreateSetLayout(Context *context,
                                                  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<GraphicsPipeline>)>>
            funcMap{
                {Pipeline_Color, MaterialColorDescriptorLogic::CreateSetLayout},
                {Pipeline_LightModel, MaterialLightModelDescriptorLogic::CreateSetLayout},
                {Pipeline_Skybox, MaterialSkyboxDescriptorLogic::CreateSetLayout},
            };

        auto &name = graphicsPipeline->name;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, graphicsPipeline);
    }
    void MaterialDescriptorLogic::DestroySetLayout(Context *context,
                                                   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<GraphicsPipeline>)>>
            funcMap{
                {Pipeline_Color, MaterialColorDescriptorLogic::DestroySetLayout},
                {Pipeline_LightModel, MaterialLightModelDescriptorLogic::DestroySetLayout},
                {Pipeline_Skybox, MaterialSkyboxDescriptorLogic::DestroySetLayout},
            };

        auto &name = graphicsPipeline->name;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, graphicsPipeline);
    }
    void MaterialDescriptorLogic::AllocateAndUpdate(Context *context,
                                                    std::shared_ptr<MaterialInstance> instance)
    {
        static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<MaterialInstance>)>>
            funcMap{
                {Pipeline_Color, MaterialColorDescriptorLogic::AllocateAndUpdate},
                {Pipeline_LightModel, MaterialLightModelDescriptorLogic::AllocateAndUpdate},
                {Pipeline_Skybox, MaterialSkyboxDescriptorLogic::AllocateAndUpdate},
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
                {Pipeline_Color, MaterialColorDescriptorLogic::Destroy},
                {Pipeline_LightModel, MaterialLightModelDescriptorLogic::Destroy},
                {Pipeline_Skybox, MaterialSkyboxDescriptorLogic::Destroy},
            };

        auto &name = instance->info->pipelineName;
        auto it = funcMap.find(name);
        if (it != funcMap.end())
            funcMap[name](context, instance);
    }
}