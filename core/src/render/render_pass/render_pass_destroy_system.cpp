
#include "render/render_pass/render_pass_logic.h"
#include "render/render_pass/render_pass_system.h"
#include "context.hpp"

namespace Render
{
    void RenderPassDestroySystem::Destroy(Context *context)
    {
        RenderPassLogic::DestroyAll(context);
    }
}