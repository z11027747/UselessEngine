
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "editor/wrap/component_wrap.h"
#include "editor/window.h"
#include "editor/test_logic.h"
#include "context.h"

namespace Editor
{
	static int objNameIndex = -1;
	static std::vector<std::string> objNames = {};
	static std::vector<const char *> objNameCStrs = {};

	static void DrawInit(Context *context,
						 std::shared_ptr<Render::Mesh> mesh)
	{
		objNameIndex = -1;
		objNames.clear();
		Window::GetDirectoryFiles("resource/obj", objNames);

		auto it = std::find(objNames.begin(), objNames.end(), mesh->info->objName);
		if (it != objNames.end())
		{
			auto index = std::distance(objNames.begin(), it);
			objNameIndex = static_cast<int>(index);
		}

		objNameCStrs.clear();
		for (auto &objName : objNames)
		{
			objNameCStrs.push_back(objName.c_str());
		}
	}

	template <>
	void ComponentWrap<Render::Mesh>::Draw(Context *context,
										   std::shared_ptr<Render::Mesh> mesh, bool isInit)
	{
		if (isInit)
		{
			DrawInit(context, mesh);
			return;
		}

		auto &meshInfo = mesh->info;

		if (ImGui::Combo("ObjName", &objNameIndex,
						 objNameCStrs.data(), static_cast<int>(objNameCStrs.size())))
		{
			meshInfo->objName = objNames[objNameIndex];
			meshInfo->hasChanged = true;
		}

		auto &meshInstance = mesh->instance;
		if (meshInstance != nullptr)
		{
			ImGui::Text("InstanceId: %d", mesh->instance->id);

			auto &boundingSphere = meshInstance->boundingSphere;

			ImGui::Spacing();
			ImGui::Text("BoundingSphere");

			ImGui_Input_GlmVec3(boundingSphere.center, 0, 90);
			ImGui::SameLine();
			ImGui::Text("Center");

			ImGui::InputFloat("Radius", &boundingSphere.radius);
		}
	}
}