#pragma once

#include <string>
#include <memory>
#include "render/vk/pass/pass_comp.h"

class Context;

namespace Render
{
    class ImGuiPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };
    class ShadowPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };
    class ForwardPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };
    class DeferredPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };
    class PostProcessPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };

    class RenderPassSystem final
    {
    public:
        static void Update(Context *, uint32_t, std::shared_ptr<Pass> pass);
    };
};