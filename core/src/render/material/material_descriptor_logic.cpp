
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/material/material_logic.h"
#include "render/material/impl/material_bling_phone_logic.h"
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
        if (name == Pipeline_Bling_Phone)
        {
            MaterialBlingPhoneDescriptorLogic::CreateSetLayout(context,
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
        if (name == Pipeline_Bling_Phone)
        {
            MaterialBlingPhoneDescriptorLogic::DestroySetLayout(context,
                                                                graphicsPipeline);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxDescriptorLogic::DestroySetLayout(context,
                                                            graphicsPipeline);
        }
    }
    void MaterialDescriptorLogic::AllocateAndUpdate(Context *context,
                                                    std::shared_ptr<Material> material)
    {
        auto &name = material->pipelineName;
        if (name == Pipeline_Bling_Phone)
        {
            MaterialBlingPhoneDescriptorLogic::AllocateAndUpdate(context,
                                                                 material);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxDescriptorLogic::AllocateAndUpdate(context,
                                                             material);
        }
    }
    void MaterialDescriptorLogic::Destroy(Context *context,
                                          std::shared_ptr<Material> material)
    {
        auto &name = material->pipelineName;
        if (name == Pipeline_Bling_Phone)
        {
            MaterialBlingPhoneDescriptorLogic::Destroy(context,
                                                       material);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxDescriptorLogic::Destroy(context,
                                                   material);
        }
    }
}