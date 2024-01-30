
#include "editor/imgui_logic.h"
#include "editor/axis/axis_system.h"
#include "editor/system.h"
#include "editor/window.h"
#include "context.h"

namespace Editor
{
	void System::Create(Context *context)
	{
		ImGuiLogic::CreateImGui(context);
		ImGuiLogic::CreateDescriptorSetLayout(context);
		ImGuiLogic::CreateDescriptors(context);
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
		ImGuiLogic::DestroyDescriptorSetLayout(context);
		ImGuiLogic::DestroyImGui(context);
	}
}