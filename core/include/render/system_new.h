#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "base.h"

class Context;

namespace Render {

	class System final {
	public:

		static void OnCreate(Context*);
		static void OnUpdate(Context*);
		static void OnDestroy(Context*);

		static void CreateEO(Context* context);
		static std::vector<const char*> GetWindowExtensions();
	};

}