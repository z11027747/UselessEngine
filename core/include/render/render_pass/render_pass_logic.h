#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pipeline/descriptor_comp.h"

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

        static void Draw(Context *, bool);
    };
}