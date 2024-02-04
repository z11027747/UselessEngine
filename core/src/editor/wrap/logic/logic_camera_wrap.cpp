
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include "render/vk/global/global_comp.h"
#include "editor/wrap/component_wrap.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
	static int mode = 0;

	static int passNameIndex = -1;
	static const char *passNames[] = {Define::Pass::Forward.c_str(),
									  Define::Pass::Deferred.c_str()};
	static const int passNameSize = 2;

	template <>
	void ComponentWrap<Logic::Camera>::Draw(Context *context,
											std::shared_ptr<Logic::Camera> camera, bool isInit)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &pass = global->passMap[camera->passName];

		if (isInit)
		{
			auto &passName = pass->name;
			for (auto i = 0; i < passNameSize; i++)
			{
				if (strcmp(passNames[i], passName.c_str()) == 0)
				{
					passNameIndex = i;
					break;
				}
			}

			mode = static_cast<int>(camera->mode);
			return;
		}

		// ImGui::Text("PassName: %s", camera->passName.data());

		if (ImGui::DragFloat("Near", &camera->near, 0.01f, 0.01f, 10.0f))
		{
			camera->far = std::max(camera->far, camera->near + 0.01f);
		}

		ImGui::DragFloat("Far", &camera->far, 0.02f, camera->near + 0.01f, 100.0f);

		if (ImGui::Combo("Mode", &mode, "Perspective\0Ortho\0"))
		{
			camera->mode = static_cast<Logic::CameraMode>(mode);
		}

		if (camera->mode == Logic::CameraMode::ePerspective)
		{
			ImGui::DragFloat("fov", &camera->fov, 0.02f, 10.0f, 90.0f);
		}
		else if (camera->mode == Logic::CameraMode::eOrtho)
		{
			ImGui::DragFloat("Size", &camera->size, 0.02f);
		}

		ImGui::Spacing();
		ImGui::Text("Render Pass");

		if (ImGui::Combo("##passNames",
						 &passNameIndex, passNames, IM_ARRAYSIZE(passNames)))
		{
			camera->passName = passNames[passNameIndex];
		}

		ImGui::Spacing();
		ImGui::Text("Clear Values");

		auto &subpasses = pass->subpasses;
		auto subpassSize = subpasses.size();
		for (auto i = 0; i < subpassSize; i++)
		{
			ImGui::Text("SubPass: %d", i);
			auto &subpass = subpasses[i];

			ImGui::PushID(i * 100);
			ImGui::ColorEdit4("Color", &subpass->clearColorValue.color.float32[0]);
			ImGui::SliderFloat2("Depth", &subpass->clearDepthValue.depthStencil.depth, 0.0f, 1.0f);
			ImGui::PopID();

			// inputs
			auto clearInputValueSize = subpass->clearInputValues.size();
			for (auto j = 0; j < clearInputValueSize; j++)
			{
				ImGui::Text("Input: %d", j);
				ImGui::PushID(j * 10);
				auto &clearInputValue = subpass->clearInputValues[j];
				ImGui::ColorEdit4("##Input", &clearInputValue.color.float32[0]);
				ImGui::PopID();
			}
		}
	}
}