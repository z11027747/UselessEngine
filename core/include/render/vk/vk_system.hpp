#pragma once

#include <cstdint>

class Context;

namespace Render
{
    class VkCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class VkDestroySystem final
    {
    public:
        static void Destroy(Context *);
    };

    class VkRenderSystem final
    {
    public:
        static uint32_t BeginUpdate(Context *);
        static void EndUpdate(Context *, uint32_t);
    };
}