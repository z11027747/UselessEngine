
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_deferred_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialDeferredPointLightPipelineLogic::SetRasterizationCreateInfo(Context *context,
                                                                             std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &stageInfo = graphicsPipeline->stageInfo;

        auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
        rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    }
    void MaterialDeferredPointLightPipelineLogic::SetDepthStencilCreateInfo(Context *context,
                                                                            std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &stageInfo = graphicsPipeline->stageInfo;

        auto &depthStencilStateCreateInfo = stageInfo.depthStencilStateCreateInfo;

        VkStencilOpState stencilOp = {};
        stencilOp.compareOp = VK_COMPARE_OP_EQUAL;
        stencilOp.failOp = VK_STENCIL_OP_KEEP;
        stencilOp.depthFailOp = VK_STENCIL_OP_KEEP;
        stencilOp.passOp = VK_STENCIL_OP_KEEP;
        stencilOp.compareMask = 0xff;
        stencilOp.writeMask = 0xff;
        stencilOp.reference = 5;

        depthStencilStateCreateInfo.stencilTestEnable = true;
        depthStencilStateCreateInfo.front = stencilOp;
        depthStencilStateCreateInfo.back = stencilOp;
    }
}