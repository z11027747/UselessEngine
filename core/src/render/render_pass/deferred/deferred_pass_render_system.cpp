
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
            auto &deferredPass = global->passMap[Define::Pass::Deferred];
            FramebufferLogic::BeginRenderPass(context, imageIndex, deferredPass);

            // subpass0: geometryPass
            RenderPassSystem::Update(context, imageIndex, false);

            FramebufferLogic::NextSubpass(context, imageIndex);

            // subpass1: lightingPass
            auto &deferredDescriptor = RenderPassLogic::descriptorMap[Define::Pass::Deferred];
            auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

            auto &deferredLightingPipeline = global->pipelineMap[Define::Pipeline::Deferred_LightModel_Lighting];

            auto &pipeline = deferredLightingPipeline->pipeline;
            auto &pipelineLayout = deferredLightingPipeline->pipelineLayout;
            vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

            std::vector<VkDescriptorSet> descriptorSets;
            descriptorSets.push_back(global->globalDescriptor->set);
            descriptorSets.push_back(deferredDescriptor->set);

            vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                    static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(),
                                    0, nullptr);

            vkCmdDraw(vkCmdBuffer, 3, 1, 0, 0);

            FramebufferLogic::EndRenderPass(context, imageIndex);
        }
    }
}