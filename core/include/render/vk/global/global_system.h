#pragma once

#include <stdint.h>

class Context;

namespace Render
{
    class GlobalUBOUpdateSystem final
    {
    public:
        static void Update(Context *);
    };

    class GlobalShadowPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };

    class GlobalMainPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };

    class GlobalImGuiPassRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t);
    };
}
