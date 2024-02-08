
#include "render/vk/vk_system.hpp"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    uint32_t VkRenderSystem::BeginUpdate(Context *context)
    {
        CmdSubmitLogic::BatchAll(context);

        BufferLogic::DestroyAllTemps(context);
        CmdPoolLogic::DestroyAllTempBuffers(context);

        SwapchainLogic::WaitFence(context);

        auto imageIndex = SwapchainLogic::AcquireImageIndex(context);
        SwapchainLogic::BeginCmd(context, imageIndex);

        return imageIndex;
    }

    void VkRenderSystem::EndUpdate(Context *context, uint32_t imageIndex)
    {
        SwapchainLogic::EndAndSubmitCmd(context, imageIndex);
        SwapchainLogic::Present(context, imageIndex);
    }
}