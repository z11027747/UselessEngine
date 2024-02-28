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

    class VkBeginRenderSystem final
    {
    public:
        static void Update(Context *);
    };

    class VkEndRenderSystem final
    {
    public:
        static void Update(Context *);
    };
}