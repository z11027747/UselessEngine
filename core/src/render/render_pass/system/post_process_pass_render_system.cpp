
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/light/light_comp.h"
#include "render/render_pass/render_pass_system.h"
#include "common/define.h"
#include "editor/system.h"
#include "engine_object.h"
#include "context.h"

class Context;

namespace Render
{
    void PostProcessPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();
        auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

        // bloom
        auto &graphicsPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Bloom];
        auto &pipeline = graphicsPipeline->pipeline;
        auto &pipelineLayout = graphicsPipeline->pipelineLayout;
        vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
        FramebufferLogic::BeginRenderPass(context, imageIndex, postProcessPass);
        {
            vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                    1, &graphicsPipeline->descriptor->set,
                                    0, nullptr);

            vkCmdDraw(vkCmdBuffer, 6, 1, 0, 0);
        }
        FramebufferLogic::EndRenderPass(context, imageIndex);
    }
}