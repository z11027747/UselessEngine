#pragma once

#include "vulkan/vulkan.h"
#include "base.h"

namespace Render {

	enum Type
	{
		eBaseInfo = 10001,
	};

	class BaseInfo final : public BaseComp {
	public:

		//vulkan实例
		VkInstance instance;


	};

}