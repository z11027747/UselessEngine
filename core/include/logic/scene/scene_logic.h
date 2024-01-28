#pragma once

class Context;

namespace Logic
{
    class SceneLogic final
    {
    public:
        static void CreateMainCamera(Context *);
        static void CreateSkybox(Context *);
        static void CreateAxis(Context *);
    };
}