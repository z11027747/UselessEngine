#pragma once

class Context;

namespace Render
{
    class UnitRenderSystem final
    {
    public:
        static void Update(Context *, uint32_t, bool);
    };

    class UnitDestroySystem final
    {
    public:
        static void Destroy(Context *);
    };
};