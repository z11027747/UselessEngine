#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/swapchain/swapchain_comp.h"

class Context;

namespace Render {

	class SwapchainSystem final {
	public:

		static void Create(Context*);

		//´´½¨½»»»Á´
		static void CreateVKSwapchain(Context*,
			std::shared_ptr<Swapchain> swapchain
		);

	};


}