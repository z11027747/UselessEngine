#pragma once

class Context;

namespace Editor
{
    class CameraMoveSystem final
    {
    public:
        static void Update(Context *);
    };

    class CameraRotateSystem final
    {
    public:
        static void Update(Context *);
    };
}