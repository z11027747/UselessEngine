
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "render/render_pass/render_pass_system.h"
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void RenderPassCreateSystem::Create(Context *context)
    {
        RenderPassLogic::CreateImGui(context);
        RenderPassLogic::CreateShadow(context);
        RenderPassLogic::CreateForward(context);
        RenderPassLogic::CreateDeferred(context);
        RenderPassLogic::CreatePostProcess(context);
    }
    void RenderPassDestroySystem::Destroy(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        for (const auto &kv : global->passMap)
        {
            auto &pass = kv.second;
            PassLogic::Destroy(context, pass);
            FramebufferLogic::DestroyColorImage2ds(context, pass);
            FramebufferLogic::DestroyDepthImage2d(context, pass);
            FramebufferLogic::DestroyResolveImage2d(context, pass);
            FramebufferLogic::DestroyInputImage2ds(context, pass);
            FramebufferLogic::Destroy(context, pass);
        }
    }
}