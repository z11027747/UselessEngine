#pragma once

#include <vector>
#include <memory>
#include "render/vk/pass/pass_comp.h"

class Context;

namespace Render
{
    class RenderPassLogic final
    {
    public:
        static std::shared_ptr<Pass> CreateImGui(Context *);
        static std::shared_ptr<Pass> CreateShadow(Context *);
        static std::shared_ptr<Pass> CreateForward(Context *);
        static std::shared_ptr<Pass> CreateDeferred(Context *);
        static std::shared_ptr<Pass> CreatePostProcess(Context *);

        static void DestroyAll(Context *);
    };
}