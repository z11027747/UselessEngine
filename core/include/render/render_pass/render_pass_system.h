#pragma once

#include <string>
#include <memory>

class Context;

namespace Render
{
    class RenderPassCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class RenderPassDestroySystem final
    {
    public:
        static void Destroy(Context *);
    };

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
        static void Update(Context *, uint32_t, bool);
    };
};