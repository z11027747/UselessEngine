#pragma once

class Context;

namespace Editor
{
    class AxisCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class AxisSelectSystem final
    {
    public:
        static bool Update(Context *);
    };
}