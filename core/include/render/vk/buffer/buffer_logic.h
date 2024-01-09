#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include "render/vk/buffer/buffer_comp.h"

class Context;

namespace Render
{
	class BufferLogic final
	{
	public:
		static std::shared_ptr<Buffer> Create(Context *,
											  VkDeviceSize, VkBufferUsageFlags,
											  VkMemoryPropertyFlags);

		static void Destroy(Context *,
							std::shared_ptr<Buffer>);

		static std::shared_ptr<Buffer> CreateTemp(Context *,
												  VkDeviceSize, VkBufferUsageFlags,
												  VkMemoryPropertyFlags);

		static void DestroyAllTemps(Context *);
	};
}
