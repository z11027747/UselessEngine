
#include "render/vk/vk_system.hpp"
#include "render/vk/global/global_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void VkDestroySystem::Destroy(Context *context)
    {
        SwapchainLogic::DestroyImageViews(context);
        SwapchainLogic::DestroySemaphores(context);
        SwapchainLogic::DestroyFences(context);
        SwapchainLogic::Destroy(context);

        DescriptorPoolLogic::Destroy(context);
        CmdPoolLogic::Destroy(context);

        LogicalDeviceLogic::Destroy(context);
        SurfaceLogic::Destroy(context);

        InstanceLogic::DestroyDebugCallback(context);
        InstanceLogic::Destroy(context);
    }
}