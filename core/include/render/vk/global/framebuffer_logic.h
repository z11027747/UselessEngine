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

		static void Update(Context*);
		static bool UpdateWaitFence(Context*);
		static uint32_t UpdateAcquireImage(Context*);
		static bool UpdateDraw(Context*, uint32_t);
		static void UpdatePresent(Context*, uint32_t);
	};


}