#pragma once

class Context;
class EngineObject;

namespace Logic
{
    class CameraUpdateVPSystem final
    {
    public:
        static void Update(Context *);
    };

    class CameraMoveSystem final
    {
    public:
        static void Update(Context *);
    };
}