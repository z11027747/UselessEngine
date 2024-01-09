#pragma once

class Context;

namespace Render
{
	class System final
	{
	public:
		static void Create(Context *);
		static void Update(Context *);
		static void Destroy(Context *);
	};

	inline static std::vector<const char *> GetWindowExtensions()
	{
		uint32_t glfwExtensionCount;
		const char **glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char *> windowExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		return windowExtensions;
	}
}