#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render {

	class FramebufferLogic final {
	public:

		static void Create(Context*);
		static void CreateFences(Context*);
		static void CreateSemaphores(Context*);

		static void Destroy(Context*);
		static void DestroyFences(Context*);
		static void DestroySemaphores(Context*);

		static void AllocateCmdBuffers(Context*);

		static void WaitFence(Context*);
		static uint32_t AcquireImageIndex(Context*);
		static void BeginRenderPass(Context*, uint32_t);
		static void DrawUnits(Context*, uint32_t);
		static void EndRenderPass(Context*, uint32_t);
		static void Present(Context*, uint32_t);
	};


}