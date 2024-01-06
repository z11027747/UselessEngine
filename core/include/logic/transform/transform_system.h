#pragma once

class Context;

namespace Logic
{
    class TransformUpdateModelSystem final
    {
    public:
        static void Update(Context *);
    };
}