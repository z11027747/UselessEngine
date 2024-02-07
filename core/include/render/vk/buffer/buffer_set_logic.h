#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	class BufferSetLogic final
	{
	public:
		template <typename T>
		static void SetPtr(Context *context,
						   std::shared_ptr<Buffer> buffer,
						   T *value, size_t size)
		{
			auto &globalEO = context->renderGlobalEO;
			auto global = globalEO->GetComponent<Global>();
			auto &logicalDevice = global->logicalDevice;

			void *data;
			auto mapRet = vkMapMemory(logicalDevice, buffer->vkDeviceMemory, 0, size, 0, &data);
			CheckRet(mapRet, "vkMapMemory");

			memcpy(data, value, size);

			vkUnmapMemory(logicalDevice, buffer->vkDeviceMemory);
		}

		template <typename T>
		static void SetPtrVector(Context *context,
								 std::shared_ptr<Buffer> buffer,
								 std::vector<T *> values, size_t size)
		{
			auto &globalEO = context->renderGlobalEO;
			auto global = globalEO->GetComponent<Global>();
			auto &logicalDevice = global->logicalDevice;

			void *data;
			auto mapRet = vkMapMemory(logicalDevice, buffer->vkDeviceMemory, 0, size, 0, &data);
			CheckRet(mapRet, "vkMapMemory");

			uint64_t memAddress;
			memAddress = reinterpret_cast<uint64_t>(data);

			for (auto i = 0; i < values.size(); i++)
			{
				auto src = reinterpret_cast<void *>(memAddress);
				auto dst = values[i];
				memcpy(src, dst, size);
				memAddress += size;
			}

			vkUnmapMemory(logicalDevice, buffer->vkDeviceMemory);
		}

		template <typename T>
		static void Set(Context *context,
						std::shared_ptr<Buffer> buffer,
						T &value)
		{
			auto &globalEO = context->renderGlobalEO;
			auto global = globalEO->GetComponent<Global>();
			auto &logicalDevice = global->logicalDevice;

			auto size = sizeof(T);

			void *data;
			auto mapRet = vkMapMemory(logicalDevice, buffer->vkDeviceMemory, 0, size, 0, &data);
			CheckRet(mapRet, "vkMapMemory");

			memcpy(data, &value, size);

			vkUnmapMemory(logicalDevice, buffer->vkDeviceMemory);
		}

		template <typename T>
		static void SetVector(Context *context,
							  std::shared_ptr<Buffer> buffer,
							  std::vector<T> &values)
		{
			auto &globalEO = context->renderGlobalEO;
			auto global = globalEO->GetComponent<Global>();
			auto &logicalDevice = global->logicalDevice;

			auto size = static_cast<size_t>(sizeof(T) * values.size());

			void *data;
			auto mapRet = vkMapMemory(logicalDevice, buffer->vkDeviceMemory, 0, size, 0, &data);
			CheckRet(mapRet, "vkMapMemory");

			memcpy(data, values.data(), size);

			vkUnmapMemory(logicalDevice, buffer->vkDeviceMemory);
		}
	};
}
