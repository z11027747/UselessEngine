#pragma once

class Context;

namespace Render
{
    class MeshCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class MeshDataUpdateSystem final
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