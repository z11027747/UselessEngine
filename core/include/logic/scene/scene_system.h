#pragma once

class Context;

namespace Logic
{
    class SceneCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class SceneChangeSystem final
    {
    public:
        static void Update(Context *);
    };
}