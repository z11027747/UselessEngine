
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_deferred_volumn_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialDeferredVolumnPipelineLogic::SetVertexAttrDescriptions(Context *context,
                                                                             std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        VkVertexInputAttributeDescription positionOSDescription = {
            0, // location
            0, // binding
            VK_FORMAT_R32G32B32_SFLOAT,
            offsetof(Render::Vertex, positionOS)};

        auto &stageInfo = graphicsPipeline->stageInfo;
        stageInfo.vertexInputAttributeDescriptions = {
            positionOSDescription};
    }
    void MaterialDeferredVolumnPipelineLogic::SetRasterizationCreateInfo(Context *context,
                                                                              std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &stageInfo = graphicsPipeline->stageInfo;

        auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
        rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    }
    void MaterialDeferredVolumnPipelineLogic::SetDepthStencilCreateInfo(Context *context,
                                                                             std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &stageInfo = graphicsPipeline->stageInfo;

        auto &depthStencilStateCreateInfo = stageInfo.depthStencilStateCreateInfo;
        depthStencilStateCreateInfo.depthTestEnable = false;
        depthStencilStateCreateInfo.depthWriteEnable = false;

        VkStencilOpState stencilOp = {};
        stencilOp.compareOp = VK_COMPARE_OP_ALWAYS;
        stencilOp.failOp = VK_STENCIL_OP_REPLACE;
        stencilOp.depthFailOp = VK_STENCIL_OP_REPLACE;
        stencilOp.passOp = VK_STENCIL_OP_REPLACE;
        stencilOp.compareMask = 0xff;
        stencilOp.writeMask = 0xff;
        stencilOp.reference = 5;
        depthStencilStateCreateInfo.stencilTestEnable = true;
        depthStencilStateCreateInfo.front = stencilOp;
        depthStencilStateCreateInfo.back = stencilOp;
    }
    void MaterialDeferredVolumnPipelineLogic::SetColorBlendStage(Context *context,
                                                                      std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &stageInfo = graphicsPipeline->stageInfo;

        auto &colorBlendAttachmentStates = stageInfo.colorBlendAttachmentStates;
        colorBlendAttachmentStates.resize(4);
        for (auto i = 0; i < 4; i++)
        {
            colorBlendAttachmentStates[i].blendEnable = false;
            colorBlendAttachmentStates[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                                           VK_COLOR_COMPONENT_G_BIT |
                                                           VK_COLOR_COMPONENT_B_BIT |
                                                           VK_COLOR_COMPONENT_A_BIT;
        }
    }
}