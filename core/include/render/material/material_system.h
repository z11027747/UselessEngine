#pragma once

class Context;

namespace Render
{
    class MaterialCreateSystem final
    {
    public:
        static void Create(Context *);
    };

    class MaterialDataUpdateSystem final
    {
    public:
        static void Update(Context *);
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
    class MaterialInstanceUpdateSystem final
    {
    public:
        static void Update(Context *);
    };

    class MaterialDestroySystem final
    {
    public:
        static void Destroy(Context *);
    };
};