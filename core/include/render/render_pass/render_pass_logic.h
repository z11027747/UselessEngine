#pragma once

#include <vector>
#include <memory>
#include "render/vk/pass/pass_comp.h"
#include "render/render_pass/render_pass_comp.h"

class Context;

namespace Render
{
    class RenderPassLogic final
    {
    public:
        static std::shared_ptr<Pass> CreateImGui(Context *);
        static std::shared_ptr<Pass> CreateMain(Context *);
        static std::shared_ptr<Pass> CreateShadow(Context *);

        static void DestroyAll(Context *);
    };
}