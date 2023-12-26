#pragma once

#include <vulkan/vulkan.h>
#include <memory>

class Context;

namespace Render {

	class SwapchainLogic final {
	public:

		static void Create(Context*);
		static void CreateColorImageViews(Context*);
		static void CreateDepthImageViews(Context*);

		static void Destroy(Context*);
		static void DestroyColorImageViews(Context*);
		static void DestroyDepthImageViews(Context*);

	};


}