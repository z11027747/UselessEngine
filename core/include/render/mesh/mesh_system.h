#pragma once

class Context;

namespace Render
{
    class MeshInstanceCreateSystem final
    {
    public:
        static void Update(Context *context);
    };

    class MeshDestroySystem final
    {
    public:
        static void Destroy(Context *context);
    };
}