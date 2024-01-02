#pragma once

#include <vulkan/vulkan.h>
#include <memory>

class Context;

namespace Render
{
	class SurfaceLogic final
	{
	public:
		static void Create(Context *);
		static void Destroy(Context *);

		static bool CheckSupport(Context *,
								 const VkPhysicalDevice &physicalDevice,
								 uint32_t queueFamilyIndex);
	};

}