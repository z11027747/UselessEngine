#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/vk/buffer/buffer_comp.h"

class Context;

namespace Render {

	class BufferLogic final {
	public:

		static std::shared_ptr<Buffer> Create(Context*,
			VkDeviceSize size, VkBufferUsageFlags usageFlags,
			VkMemoryPropertyFlags propertiesFlags);

		static void Destroy(Context*, 
			std::shared_ptr<Buffer> buffer);

	};
}
