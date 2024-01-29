#pragma once

#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Context;

namespace Editor
{
	class System final
	{
	public:
		static void Create(Context *);
		static void Update(Context *);
		static void Destroy(Context *);
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

	class CameraRotateSystem final
	{
	public:
		static void Update(Context *);
	};

	class EOSelectSystem final
	{
	public:
		static void Update(Context *);
	};
}