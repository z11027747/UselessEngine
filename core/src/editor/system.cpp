
#include "editor/test_logic.h"
#include "editor/system.h"
#include "editor/window.h"
#include "context.h"

namespace Editor
{
	VkDescriptorSetLayout System::descriptorSetLayout = nullptr;
	std::shared_ptr<Render::Descriptor> System::descriptor = nullptr;

	void System::Create(Context *context)
	{
		TestLogic::CreateImGui(context);
		TestLogic::CreateDescriptor(context);
	}

	void System::Update(Context *context)
	{
		CameraMoveSystem::Update(context);
		CameraRotateSystem::Update(context);
		AxisSelectSystem::Update(context);
		EOSelectSystem::Update(context);
	}

	void System::Destroy(Context *context)
	{
		TestLogic::DestroyDescriptor(context);
		TestLogic::DestroyImGui(context);
	}
}