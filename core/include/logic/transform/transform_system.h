#pragma once

class Context;

namespace Logic
{
    class TransformCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class TransformUpdateMSystem final
    {
    public:
        static void Update(Context *);
    };
}