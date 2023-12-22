#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include "base.h"

class Context;

namespace Render {

	class System final {
	public:

		static void OnCreate(Context*);
		static void OnUpdate(Context*);
		static void OnDestroy(Context*);

	};

}