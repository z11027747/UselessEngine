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
        static void Update(Context *);
    };
    class ShadowPassRenderSystem final
    {
    public:
        static void Update(Context *);
    };
    class ForwardPassRenderSystem final
    {
    public:
        static void Update(Context *);
    };
    class DeferredPassRenderSystem final
    {
    public:
        static void Update(Context *);
    };
    class PostProcessPassRenderSystem final
    {
    public:
        static void Update(Context *);
    };
};