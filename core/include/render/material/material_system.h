#pragma once

class Context;

namespace Render
{
    class MaterialGlobalUBOUpdateSystem final
    {
    public:
        static void Update(Context *);
    };
};