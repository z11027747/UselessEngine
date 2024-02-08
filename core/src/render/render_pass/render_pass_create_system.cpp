
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
}