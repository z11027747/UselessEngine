#pragma once

class Context;

namespace Logic
{
    class CameraCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class CameraUpdateVPSystem final
    {
    public:
        static void Update(Context *);
    };
}