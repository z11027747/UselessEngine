#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Editor {

	class Window final {
	public:

		static void Create(Context*);
		static void CreateStyle(Context*);
		static void Destroy(Context*);

		static void Update(Context*);
		static void DrawMenuBar(Context*);
		static void DrawHierarchy(Context*);
		static void DrawInspector(Context*);

		static void DrawData(Context*, uint32_t);

	};

}