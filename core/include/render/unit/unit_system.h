#pragma once

#include <stdint.h>

class Context;

namespace Render
{
    class ShadowPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };

    class MainPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };

    class ImGuiPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };

    class UnitRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t, bool);
    };
};