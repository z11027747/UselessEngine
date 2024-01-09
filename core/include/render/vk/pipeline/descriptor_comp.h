#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace Render
{
	struct Descriptor final
	{
		VkDescriptorSet set;
		VkDescriptorBufferInfo bufferInfo;
		VkDescriptorImageInfo image0Info;
		VkDescriptorImageInfo image1Info;
	};

	struct CameraUBO final
	{
		alignas(16) glm::vec3 pos;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
	};

	struct DirectionLightUBO final
	{
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
		alignas(16) glm::vec3 dir;
		alignas(16) glm::vec3 col;
		alignas(16) glm::vec4 params;
	};

	struct GlobalUBO final
	{
		alignas(16) CameraUBO camera;
		alignas(16) DirectionLightUBO directionLight;
	};
}