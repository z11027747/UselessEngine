#pragma once

class Context;

namespace Logic
{
    class RotateCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class RotateAroundSystem final
    {
    public:
        static void Update(Context *);
    };
}
