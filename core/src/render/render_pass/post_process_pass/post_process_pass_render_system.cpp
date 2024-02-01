
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/light/light_comp.h"
#include "render/render_pass/render_pass_system.h"
#include "render/post_process/post_process_comp.h"
#include "render/post_process/post_process_logic.h"
#include "common/define.h"
#include "editor/system.h"
#include "engine_object.h"
#include "context.h"

class Context;

namespace Render
{
    static void DrawPipeline(VkCommandBuffer vkCmdBuffer,
                             std::shared_ptr<GraphicsPipeline> graphicsPipeline,
                             std::shared_ptr<Descriptor> descriptor, glm::vec4 params)
    {
        auto &pipeline = graphicsPipeline->pipeline;
        auto &pipelineLayout = graphicsPipeline->pipelineLayout;
        vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                1, &descriptor->set,
                                0, nullptr);

        vkCmdPushConstants(vkCmdBuffer, pipelineLayout,
                           VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec4), &params);

        vkCmdDraw(vkCmdBuffer, 6, 1, 0, 0);
    }

    void PostProcessPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();
        auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

        auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
        FramebufferLogic::BeginRenderPass(context, imageIndex, postProcessPass);

        auto &mainCameraEO = context->logicMainCameraEO;
        PostProcessLogic::Blit(context, mainCameraEO);

        auto postProcess = mainCameraEO->GetComponent<PostProcess>();

        // toon mapping
        if (postProcess->toonMappingParams.w == 1.0f)
        {
            auto &graphicsPipeline = global->pipelineMap[Define::Pipeline::PostProcess_ToonMapping];
            DrawPipeline(vkCmdBuffer,
                         graphicsPipeline,
                         postProcess->descriptor, postProcess->toonMappingParams);
        }

        // bloom
        if (postProcess->bloomParams.w == 1.0f)
        {
            auto &graphicsPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Bloom];
            DrawPipeline(vkCmdBuffer,
                         graphicsPipeline,
                         postProcess->descriptor, postProcess->bloomParams);
        }

        FramebufferLogic::EndRenderPass(context, imageIndex);
    }
}