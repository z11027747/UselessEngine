
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
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Color)
        {
            MaterialColorDescriptorLogic::CreateSetLayout(context,
                                                          graphicsPipeline);
        }
        else if (name == Pipeline_LightModel)
        {
            MaterialLightModelDescriptorLogic::CreateSetLayout(context,
                                                               graphicsPipeline);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxDescriptorLogic::CreateSetLayout(context,
                                                           graphicsPipeline);
        }
    }
    void MaterialDescriptorLogic::DestroySetLayout(Context *context,
                                                   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Color)
        {
            MaterialColorDescriptorLogic::DestroySetLayout(context,
                                                           graphicsPipeline);
        }
        else if (name == Pipeline_LightModel)
        {
            MaterialLightModelDescriptorLogic::DestroySetLayout(context,
                                                                graphicsPipeline);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxDescriptorLogic::DestroySetLayout(context,
                                                            graphicsPipeline);
        }
    }
    void MaterialDescriptorLogic::AllocateAndUpdate(Context *context,
                                                    std::shared_ptr<MaterialInstance> instance)
    {
        auto &name = instance->info->pipelineName;
        if (name == Pipeline_Color)
        {
            MaterialColorDescriptorLogic::AllocateAndUpdate(context,
                                                            instance);
        }
        else if (name == Pipeline_LightModel)
        {
            MaterialLightModelDescriptorLogic::AllocateAndUpdate(context,
                                                                 instance);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxDescriptorLogic::AllocateAndUpdate(context,
                                                             instance);
        }
    }
    void MaterialDescriptorLogic::Destroy(Context *context,
                                          std::shared_ptr<MaterialInstance> instance)
    {
        auto &name = instance->info->pipelineName;
        if (name == Pipeline_Color)
        {
            MaterialColorDescriptorLogic::Destroy(context,
                                                  instance);
        }
        else if (name == Pipeline_LightModel)
        {
            MaterialLightModelDescriptorLogic::Destroy(context,
                                                       instance);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxDescriptorLogic::Destroy(context,
                                                   instance);
        }
    }
}