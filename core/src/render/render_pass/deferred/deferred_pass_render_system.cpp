
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/material/material_comp.h"
#include "render/render_pass/render_pass_system.h"
#include "render/render_pass/render_pass_logic.h"
#include "logic/camera/camera_comp.h"
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

class Context;

namespace Render
{
    inline static void DrawPipeline(VkCommandBuffer vkCmdBuffer,
                                    std::shared_ptr<GraphicsPipeline> graphicsPipeline,
                                    std::shared_ptr<Descriptor> globalDescriptor, std::shared_ptr<Descriptor> descriptor)
    {
        auto &pipeline = graphicsPipeline->pipeline;
        auto &pipelineLayout = graphicsPipeline->pipelineLayout;
        vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        std::vector<VkDescriptorSet> descriptorSets;
        descriptorSets.push_back(globalDescriptor->set);
        descriptorSets.push_back(descriptor->set);

        vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(),
                                0, nullptr);

        vkCmdDraw(vkCmdBuffer, 3, 1, 0, 0);
    }

    void DeferredPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &mainCameraEO = context->logicMainCameraEO;
        if (mainCameraEO == nullptr)
            return;

        auto mainCamera = mainCameraEO->GetComponent<Logic::Camera>();
        if (mainCameraEO->active && mainCamera->passName == Define::Pass::Deferred)
        {
            auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];
            auto &deferredPass = global->passMap[Define::Pass::Deferred];

            auto instanceCache = globalEO->GetComponent<MaterialInstanceCache>();
            auto &deferredDescriptor = instanceCache->globalInstanceMap[Define::Pass::Deferred]->descriptor;

            // subpass0: geometryPass
            FramebufferLogic::BeginRenderPass(context, imageIndex, deferredPass);
            {
                RenderPassLogic::Draw(context, imageIndex, false);
            }

            // subpass1: pointlightPass
            FramebufferLogic::NextSubpass(context, imageIndex);
            {
                auto &deferredPointLightPipeline = global->pipelineMap[Define::Pipeline::Deferred_PointLight];
                DrawPipeline(vkCmdBuffer,
                             deferredPointLightPipeline,
                             global->globalDescriptor, deferredDescriptor);
            }

            // subpass2: shadingPass
            FramebufferLogic::NextSubpass(context, imageIndex);
            {
                auto &deferredShadingPipeline = global->pipelineMap[Define::Pipeline::Deferred_Shading];
                DrawPipeline(vkCmdBuffer,
                             deferredShadingPipeline,
                             global->globalDescriptor, deferredDescriptor);
            }

            FramebufferLogic::EndRenderPass(context, imageIndex);
        }
    }
}