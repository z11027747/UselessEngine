
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/cmd/cmd_logic.h"
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
    static void BlitResolveImageAndGenMipmaps(Context *);

    static void DrawPipeline(VkCommandBuffer,
                             std::shared_ptr<GraphicsPipeline>,
                             std::shared_ptr<Descriptor>, glm::vec4 &);

    void PostProcessPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &mainCameraEO = context->logicMainCameraEO;
        auto postProcess = mainCameraEO->GetComponent<PostProcess>();
        if (postProcess == nullptr)
            return;

        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();
        auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

        auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
        FramebufferLogic::BeginRenderPass(context, imageIndex, postProcessPass);

        // toon mapping
        auto &toonMappingParams = postProcess->toonMappingParams;
        if (toonMappingParams.w == 1.0f)
        {
            auto &toonMappingPipeline = global->pipelineMap[Define::Pipeline::PostProcess_ToonMapping];
            DrawPipeline(vkCmdBuffer,
                         toonMappingPipeline,
                         postProcess->descriptor, toonMappingParams);
        }

        FramebufferLogic::NextSubpass(context, imageIndex);

        // bloom
        auto &bloomParams = postProcess->bloomParams;
        if (bloomParams.w == 1.0f)
        {
            BlitResolveImageAndGenMipmaps(context);

            auto &graphicsPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Bloom];
            DrawPipeline(vkCmdBuffer,
                         graphicsPipeline,
                         postProcess->descriptor, bloomParams);
        }

        FramebufferLogic::NextSubpass(context, imageIndex);

        auto &globalPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Global];
        DrawPipeline(vkCmdBuffer,
                     globalPipeline,
                     postProcess->descriptor, glm::vec4(0.0f));

        FramebufferLogic::EndRenderPass(context, imageIndex);
    }

    static void BlitResolveImageAndGenMipmaps(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &mainCameraEO = context->logicMainCameraEO;
        auto postProcess = mainCameraEO->GetComponent<PostProcess>();

        auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
        auto &colorImage2d = postProcessPass->colorImage2ds[1];

        auto &mainPass = global->passMap[Define::Pass::Main];
        auto &resolveImage2d = mainPass->resolveImage2d;

        ImageLogic::TransitionLayout(context, resolveImage2d,
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 1);
        ImageLogic::TransitionLayout(context, colorImage2d,
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, colorImage2d->mipLevels);

        ImageLogic::CopyFromImage(context, colorImage2d,
                                  resolveImage2d);

        ImageLogic::TransitionLayout(context, resolveImage2d,
                                     VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);

        ImageLogic::GenerateMipmapsAndTransitionLayout(context, colorImage2d,
                                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        CmdSubmitLogic::BatchAll(context);
    }
    static void DrawPipeline(VkCommandBuffer vkCmdBuffer,
                             std::shared_ptr<GraphicsPipeline> graphicsPipeline,
                             std::shared_ptr<Descriptor> descriptor, glm::vec4 &params)
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
}