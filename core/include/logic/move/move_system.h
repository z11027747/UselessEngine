#pragma once

class Context;

namespace Logic
{
    class MoveCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class MoveFollowSystem final
    {
    public:
        static void Update(Context *);
    };
}
