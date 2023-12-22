#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/buffer/buffer_comp.h"

class Context;

namespace Render {

	class BufferSystem final {
	public:

		//创建缓冲
		static std::shared_ptr<Buffer> Create(Context*,
			VkDeviceSize size, VkBufferUsageFlags usageFlags,
			VkMemoryPropertyFlags propertiesFlags);

		//销毁缓冲
		static void Destroy(Context*, 
			std::shared_ptr<Buffer> buffer);

	};
}
