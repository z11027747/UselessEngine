#pragma once

#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "render/vk/pipeline/descriptor_comp.h"

class Context;

namespace Editor
{
	class System final
	{
	public:
		static void Create(Context *);
		static void Destroy(Context *);

		static VkDescriptorSetLayout descriptorSetLayout;
		static std::vector<std::shared_ptr<Render::Descriptor>> descriptors;
		static void CreateDescriptors(Context *);
		static void DestroyDescriptors(Context *);

		static void NewFrame(Context *);
		static void Render(Context *, uint32_t);
	};
}