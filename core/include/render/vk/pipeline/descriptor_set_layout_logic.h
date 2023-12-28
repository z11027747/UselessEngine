#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>

class Context;

namespace Render {

	class DescriptorSetLayoutLogic final {
	public:

		static VkDescriptorSetLayout Create(Context*,
			std::vector<VkDescriptorSetLayoutBinding>&);

		static void Destroy(Context*, VkDescriptorSetLayout);

	};


}