#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Context;

namespace Render
{
	class InstanceLogic final
	{
	public:
		static void Create(Context *,
						   std::vector<const char *> windowExtensions,
						   bool enabledDebug);
		static void Destroy(Context *);

		static void CreateDebugCallback(Context *);
		static void DestroyDebugCallback(Context *);

		static uint32_t GetApiVersion();
		static bool CheckExtension(const char *);
		static bool CheckLayer(const char *);
	};
}