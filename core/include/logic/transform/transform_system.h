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

    // Move =====================================
    class MoveCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class MoveForwardSystem final
    {
    public:
        static void Update(Context *);
    };
    class MoveFollowSystem final
    {
    public:
        static void Update(Context *);
    };

    // Rotate =====================================
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

    // Scale =====================================
    class ScaleCreateSystem final
    {
    public:
        static void Create(Context *);
    };
    class ScaleKeepSystem final
    {
    public:
        static void Update(Context *);
    };
}