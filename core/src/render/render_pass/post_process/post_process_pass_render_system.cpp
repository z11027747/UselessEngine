
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/render_pass/render_pass_system.h"
#include "render/render_pass/render_pass_logic.h"
#include "render/post_process/post_process_comp.h"
#include "render/post_process/post_process_logic.h"
#include "logic/camera/camera_comp.h"
#include "define.hpp"
#include "editor/system.h"
#include "engine_object.hpp"
#include "context.hpp"

class Context;

namespace Render
{
    inline static void BlitResolveImageAndGenMipmaps(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &mainCameraEO = context->logicMainCameraEO;

        auto camera = mainCameraEO->GetComponent<Logic::Camera>();
        auto &cameraPass = camera->passName;
        auto &dstImage2d = global->passMap[cameraPass]->resolveImage2d;

        auto postProcess = mainCameraEO->GetComponent<PostProcess>();
        auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
        auto &srcImage2d = postProcessPass->colorImage2ds[1];

        ImageLogic::TransitionLayout(context, dstImage2d,
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 1);
        ImageLogic::TransitionLayout(context, srcImage2d,
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, srcImage2d->mipLevels);

        ImageLogic::CopyFromImage(context, srcImage2d,
                                  dstImage2d);

        ImageLogic::TransitionLayout(context, dstImage2d,
                                     VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);

        ImageLogic::GenerateMipmapsAndTransitionLayout(context, srcImage2d,
                                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        CmdSubmitLogic::BatchAll(context);
    }
    inline static void DrawPipeline(VkCommandBuffer vkCmdBuffer,
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

        vkCmdDraw(vkCmdBuffer, 3, 1, 0, 0);
    }

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

        auto instanceCache = globalEO->GetComponent<MaterialInstanceCache>();
        auto &postProcessDescriptor = instanceCache->globalInstanceMap[Define::Pass::PostProcess]->descriptor;

        // toon mapping
        auto &toonMappingParams = postProcess->toonMappingParams;
        if (toonMappingParams.w == 1.0f)
        {
            auto &toonMappingPipeline = global->pipelineMap[Define::Pipeline::PostProcess_ToonMapping];
            DrawPipeline(vkCmdBuffer,
                         toonMappingPipeline,
                         postProcessDescriptor, toonMappingParams);
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
                         postProcessDescriptor, bloomParams);
        }

        FramebufferLogic::NextSubpass(context, imageIndex);

        auto &globalPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Global];
        DrawPipeline(vkCmdBuffer,
                     globalPipeline,
                     postProcessDescriptor, glm::vec4(0.0f));

        FramebufferLogic::EndRenderPass(context, imageIndex);
    }
}