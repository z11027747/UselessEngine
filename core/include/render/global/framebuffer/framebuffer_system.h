#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render {

	class FramebufferSystem final {
	public:

		static void Create(Context*);
		static void Destroy(Context*);

		static void CreateFences(Context*);
		static void DestroyFences(Context*);

		static void CreateSemaphores(Context*);
		static void DestroySemaphores(Context*);

	};


}