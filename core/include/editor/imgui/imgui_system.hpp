#pragma once

#include <cstdint>

class Context;

namespace Editor
{
    class ImGuiCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class ImGuiDestroySystem final
    {
    public:
        static void Destroy(Context *);
    };

    class ImGuiRenderPassLogic final
    {
    public:
        static void Draw(Context *, uint32_t);
    };
}