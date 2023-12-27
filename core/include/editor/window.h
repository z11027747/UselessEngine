#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Editor {

	class Window final {
	public:

		static void Create(Context*);
		static void Update(Context*);
		static void DrawData(Context*, uint32_t);

	};

}