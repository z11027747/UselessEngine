#pragma once

class Context;

namespace Render
{
    class MaterialInstanceCreateSystem final
    {
    public:
        static void Update(Context *context);
    };

    class MaterialDestroySystem final
    {
    public:
        static void Destroy(Context *context);
    };

    class MaterialUBOUpdateSystem final
    {
    public:
        static void Update(Context *);
    };

    class MaterialGlobalUBOUpdateSystem final
    {
    public:
        static void Update(Context *);
    };
};