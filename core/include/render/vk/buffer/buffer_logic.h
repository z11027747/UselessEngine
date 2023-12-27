#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include "render/vk/buffer/buffer_comp.h"

//TODO 为了模板实现
#include "context.h"
#include "render/vk/global/global_system.h"

class Context;

namespace Render {

	class BufferLogic final {
	public:

		static std::shared_ptr<Buffer> Create(Context*,
			VkDeviceSize, VkBufferUsageFlags,
			VkMemoryPropertyFlags);

		static void Destroy(Context*,
			std::shared_ptr<Buffer>);

		template<typename T>
		static void SetPtr(Context* context,
			std::shared_ptr<Buffer> buffer,
			T* value, size_t size
		) {
			auto& renderGlobalEO = context->renderGlobalEO;

			auto global = renderGlobalEO->GetComponent<Global>();
			auto& logicalDevice = global->logicalDevice;

			void* data;
			auto mapRet = vkMapMemory(logicalDevice, buffer->vkDeviceMemory, 0, size, 0, &data);
			CheckRet(mapRet, "vkMapMemory");

			memcpy(data, value, size);

			vkUnmapMemory(logicalDevice, buffer->vkDeviceMemory);
		}

		template<typename T>
		static void SetValue(Context* context,
			std::shared_ptr<Buffer> buffer,
			T& value
		) {
			auto& renderGlobalEO = context->renderGlobalEO;

			auto global = renderGlobalEO->GetComponent<Global>();
			auto& logicalDevice = global->logicalDevice;

			auto size = sizeof(value);

			void* data;
			auto mapRet = vkMapMemory(logicalDevice, buffer->vkDeviceMemory, 0, size, 0, &data);
			CheckRet(mapRet, "vkMapMemory");

			memcpy(data, &value, size);

			vkUnmapMemory(logicalDevice, buffer->vkDeviceMemory);
		}

		template<typename T>
		static void SetValues(Context* context,
			std::shared_ptr<Buffer> buffer,
			std::vector<T>& values
		) {
			auto& renderGlobalEO = context->renderGlobalEO;

			auto global = renderGlobalEO->GetComponent<Global>();
			auto& logicalDevice = global->logicalDevice;

			auto size = static_cast<size_t>(sizeof(values[0]) * values.size());

			void* data;
			auto mapRet = vkMapMemory(logicalDevice, buffer->vkDeviceMemory, 0, size, 0, &data);
			CheckRet(mapRet, "vkMapMemory");

			memcpy(data, values.data(), size);

			vkUnmapMemory(logicalDevice, buffer->vkDeviceMemory);
		}

	};
}
