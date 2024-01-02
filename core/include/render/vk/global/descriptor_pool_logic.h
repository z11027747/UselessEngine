#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render
{
	class DescriptorPoolLogic final
	{
	public:
		static void Create(Context *);
		static void Destroy(Context *);
	};
}