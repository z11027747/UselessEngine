#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render {

	class FramebufferSystem final {
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
		static bool UpdateAcquireNext(Context*, uint32_t&);
		static bool UpdateSubmitCmds(Context*, uint32_t);
		static void UpdatePresent(Context*, uint32_t);
	};


}