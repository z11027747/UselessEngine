
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/render_pass/render_pass_system.h"
#include "render/render_pass/render_pass_logic.h"
#include "logic/camera/camera_comp.h"
#include "common/define.h"
#include "engine_object.h"
#include "context.h"

class Context;

namespace Render
{
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
            auto &deferredDescriptor = RenderPassLogic::descriptorMap[Define::Pass::Deferred];

            // subpass0: geometryPass
            FramebufferLogic::BeginRenderPass(context, imageIndex, deferredPass);
            {
                RenderPassSystem::Update(context, imageIndex, false);
            }

            // subpass1: pointlightPass
            FramebufferLogic::NextSubpass(context, imageIndex);
            {
                auto &deferredPointLightPipeline = global->pipelineMap[Define::Pipeline::Deferred_PointLight];
                auto &pipeline = deferredPointLightPipeline->pipeline;
                auto &pipelineLayout = deferredPointLightPipeline->pipelineLayout;
                vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

                std::vector<VkDescriptorSet> descriptorSets;
                descriptorSets.push_back(global->globalDescriptor->set);
                descriptorSets.push_back(deferredDescriptor->set);

                vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                        static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(),
                                        0, nullptr);

                vkCmdDraw(vkCmdBuffer, 3, 1, 0, 0);
            }

            // subpass2: shadingPass
            FramebufferLogic::NextSubpass(context, imageIndex);
            {
                auto &deferredShadingPipeline = global->pipelineMap[Define::Pipeline::Deferred_Shading];
                auto &pipeline = deferredShadingPipeline->pipeline;
                auto &pipelineLayout = deferredShadingPipeline->pipelineLayout;
                vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

                std::vector<VkDescriptorSet> descriptorSets;
                descriptorSets.push_back(global->globalDescriptor->set);
                descriptorSets.push_back(deferredDescriptor->set);

                vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                        static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(),
                                        0, nullptr);

                vkCmdDraw(vkCmdBuffer, 3, 1, 0, 0);
            }

            FramebufferLogic::EndRenderPass(context, imageIndex);
        }
    }
}