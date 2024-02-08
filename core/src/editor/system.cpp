
#include "editor/axis/axis_system.h"
#include "editor/camera/camera_system.hpp"
#include "editor/imgui/imgui_logic.h"
#include "editor/imgui/imgui_system.hpp"
#include "editor/wrap/wrap_system.hpp"
#include "editor/system.h"
#include "context.hpp"

namespace Editor
{
	void System::Create(Context *context)
	{
		WrapRegistSystem::Create(context);
		ImGuiCreateSystem::Create(context);
	}
	void System::Update(Context *context)
	{
		CameraMoveSystem::Update(context);
		CameraRotateSystem::Update(context);
		auto result = AxisSelectSystem::Update(context);
		if (!result)
		{
			EOSelectSystem::Update(context);
		}
	}
	void System::Destroy(Context *context)
	{
		ImGuiDestroySystem::Destroy(context);
	}
}