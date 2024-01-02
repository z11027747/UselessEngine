#pragma once

#include <vulkan/vulkan.h>
#include <memory>

class Context;

namespace Render
{
	class SwapchainLogic final
	{
	public:
		static void Create(Context *);
		static void Destroy(Context *);

		static void CreateImageViews(Context *);
		static void DestroyImageViews(Context *);

		static void CreateFences(Context *);
		static void DestroyFences(Context *);

		static void CreateSemaphores(Context *);
		static void DestroySemaphores(Context *);

		static void WaitFence(Context *);
		static uint32_t AcquireImageIndex(Context *);

		static void AllocateCmd(Context *);
		static VkCommandBuffer &BeginCmd(Context *, uint32_t);
		static void EndAndSubmitCmd(Context *, uint32_t);
		static void Present(Context *, uint32_t);
	};

}