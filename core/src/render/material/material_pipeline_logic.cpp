
#include <vulkan/vulkan.h>
#include "render/mesh/mesh_comp.h"
#include "render/material/material_logic.h"
#include "render/material/impl/material_bling_phone_logic.h"
#include "render/material/impl/material_color_logic.h"
#include "render/material/impl/material_shadow_logic.h"
#include "render/material/impl/material_skybox_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    void MaterialPipelineLogic::SetVertexAttrDescriptions(Context *context,
                                                          std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Bling_Phone)
        {
            MaterialBlingPhonePipelineLogic::SetVertexAttrDescriptions(context,
                                                                       graphicsPipeline);
        }
        else if (name == Pipeline_Color)
        {
            MaterialColorPipelineLogic::SetVertexAttrDescriptions(context,
                                                                  graphicsPipeline);
        }
        else if (name == Pipeline_Shadow)
        {
            MaterialShadowPipelineLogic::SetVertexAttrDescriptions(context,
                                                                   graphicsPipeline);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxPipelineLogic::SetVertexAttrDescriptions(context,
                                                                   graphicsPipeline);
        }
    }
    void MaterialPipelineLogic::SetViewport(Context *context,
                                            std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Shadow)
        {
            MaterialShadowPipelineLogic::SetViewport(context,
                                                     graphicsPipeline);
        }
    }
    void MaterialPipelineLogic::SetRasterizationCreateInfo(Context *context,
                                                           std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Bling_Phone)
        {
            MaterialBlingPhonePipelineLogic::SetRasterizationCreateInfo(context,
                                                                        graphicsPipeline);
        }
        else if (name == Pipeline_Color)
        {
            MaterialColorPipelineLogic::SetRasterizationCreateInfo(context,
                                                                   graphicsPipeline);
        }
        else if (name == Pipeline_Shadow)
        {
            MaterialShadowPipelineLogic::SetRasterizationCreateInfo(context,
                                                                    graphicsPipeline);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxPipelineLogic::SetRasterizationCreateInfo(context,
                                                                    graphicsPipeline);
        }
    }
    void MaterialPipelineLogic::SetDepthStencilCreateInfo(Context *context,
                                                          std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &name = graphicsPipeline->name;
        if (name == Pipeline_Color)
        {
            MaterialColorPipelineLogic::SetDepthStencilCreateInfo(context,
                                                                  graphicsPipeline);
        }
        else if (name == Pipeline_Skybox)
        {
            MaterialSkyboxPipelineLogic::SetDepthStencilCreateInfo(context,
                                                                   graphicsPipeline);
        }
    }
    void MaterialPipelineLogic::SetColorBlendStage(Context *context,
                                                   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
    }
}