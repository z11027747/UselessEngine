
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
    // 兼容 forward和deferred pass
    inline static void BlitResolveImage(Context *context)
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
        ImageLogic::TransitionLayout(context, srcImage2d,
                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, srcImage2d->mipLevels);

        CmdSubmitLogic::BatchAll(context);
    }
    inline static void DrawPipeline(Context *context, uint32_t imageIndex,
                                    std::shared_ptr<GraphicsPipeline> graphicsPipeline, glm::vec4 &params)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();
        auto &globalDescriptor = global->globalDescriptor;
        auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

        auto &pipeline = graphicsPipeline->pipeline;
        auto &pipelineLayout = graphicsPipeline->pipelineLayout;
        vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        auto instanceCache = globalEO->GetComponent<MaterialInstanceCache>();
        auto &instanceDescriptor = instanceCache->globalInstanceMap[graphicsPipeline->name]->descriptor;

        std::vector<VkDescriptorSet> descriptorSets;
        descriptorSets.push_back(globalDescriptor->set);
        descriptorSets.push_back(instanceDescriptor->set);
        vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(),
                                0, nullptr);

        vkCmdPushConstants(vkCmdBuffer, pipelineLayout,
                           VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(glm::vec4), &params);

        vkCmdDraw(vkCmdBuffer, 3, 1, 0, 0);
    }
    inline static void DrawPipeline(Context *context, uint32_t imageIndex,
                                    std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        DrawPipeline(context, imageIndex,
                     graphicsPipeline, glm::vec4());
    }

    void PostProcessPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &mainCameraEO = context->logicMainCameraEO;
        auto postProcess = mainCameraEO->GetComponent<PostProcess>();
        if (postProcess == nullptr)
            return;

        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();
        auto &postProcessPass = global->passMap[Define::Pass::PostProcess];

        BlitResolveImage(context);

        // SSAO
        FramebufferLogic::BeginRenderPass(context, imageIndex, postProcessPass);
        {
            auto &SSAOParams = postProcess->SSAOParams;
            auto &SSAOPipeline = global->pipelineMap[Define::Pipeline::PostProcess_SSAO];
            DrawPipeline(context, imageIndex,
                         SSAOPipeline, SSAOParams);
        }

        // toon mapping
        FramebufferLogic::NextSubpass(context, imageIndex);
        {
            auto &toonMappingParams = postProcess->toonMappingParams;
            auto &toonMappingPipeline = global->pipelineMap[Define::Pipeline::PostProcess_ToonMapping];
            DrawPipeline(context, imageIndex,
                         toonMappingPipeline, toonMappingParams);
        }

        // gauss blur
        FramebufferLogic::NextSubpass(context, imageIndex);
        {
            auto &gaussBlurParams = postProcess->gaussBlurParams;
            auto &gaussBlurPipeline = global->pipelineMap[Define::Pipeline::PostProcess_GaussBlur];
            DrawPipeline(context, imageIndex,
                         gaussBlurPipeline, gaussBlurParams);
        }

        // bloom
        FramebufferLogic::NextSubpass(context, imageIndex);
        {
            auto &bloomParams = postProcess->bloomParams;
            auto &bloomPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Bloom];
            DrawPipeline(context, imageIndex,
                         bloomPipeline, bloomParams);
        }

        // global
        FramebufferLogic::NextSubpass(context, imageIndex);
        {
            auto &globalPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Global];
            DrawPipeline(context, imageIndex,
                         globalPipeline);
        }

        FramebufferLogic::EndRenderPass(context, imageIndex);
    }
}