#pragma once

#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "render/vk/pipeline/descriptor_comp.h"
#include "logic/transform/transform_comp.h"

class Context;

namespace Editor
{
	class System final
	{
	public:
		static void Create(Context *);
		static void Update(Context *);
		static void Destroy(Context *);

		static VkDescriptorSetLayout descriptorSetLayout;
		static std::shared_ptr<Render::Descriptor> descriptor;
	};

	class RenderSystem final
	{
	public:
		static void Update(Context *, uint32_t);
	};

	class CameraMoveSystem final
	{
	public:
		static void Update(Context *);
	};

	class ViewportSelectSystem final
	{
	public:
		static void Update(Context *);
	};
}