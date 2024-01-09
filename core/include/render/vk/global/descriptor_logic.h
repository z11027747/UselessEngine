#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render
{
    class DescriptorLogic final
    {
    public:
        static void CreateGlobal(Context *);
        static void DestroyGlobal(Context *);
    };
}