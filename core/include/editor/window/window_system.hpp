#pragma once

class Context;

namespace Editor
{
    class WindowMenuBarSystem final
    {
    public:
        static void Update(Context *);
    };
    class WindowHierachySystem final
    {
    public:
        static void Update(Context *);
    };
    class WindowInspectorSystem final
    {
    public:
        static void Update(Context *);
    };
    class WindowViewportSystem final
    {
    public:
        static void Update(Context *);
    };
    class WindowConsoleSystem final
    {
    public:
        static void Update(Context *);
    };
    class WindowProjectSystem final
    {
    public:
        static void Update(Context *);
    };
}