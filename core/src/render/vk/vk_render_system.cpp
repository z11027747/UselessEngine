
#include "render/vk/vk_system.hpp"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void VkBeginRenderSystem::Update(Context *context)
    {
        CmdSubmitLogic::BatchAll(context);
        BufferLogic::DestroyAllTemps(context);
        CmdPoolLogic::DestroyAllTempBuffers(context);

        SwapchainLogic::WaitFence(context);

        SwapchainLogic::AcquireImageIndex(context);
        SwapchainLogic::BeginCmd(context);
    }

    void VkEndRenderSystem::Update(Context *context)
    {
        SwapchainLogic::EndAndSubmitCmd(context);
        SwapchainLogic::Present(context);
    }
}