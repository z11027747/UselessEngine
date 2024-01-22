
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

	template <>
	void ComponentWrap<Render::Mesh>::Draw(Context *context,
										   std::shared_ptr<Render::Mesh> mesh, bool isFirst)
	{
		if (isFirst)
		{
			objNameIndex = -1;
			objNames.clear();
			Window::GetDirectoryFiles("resource/obj", objNames);

			auto it = std::find(objNames.begin(), objNames.end(), mesh->objName);
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

			return;
		}

		if (ImGui::Combo("ObjName", &objNameIndex,
						 objNameCStrs.data(), static_cast<int>(objNameCStrs.size())))
		{
			mesh->objName = objNames[objNameIndex];
			mesh->hasChanged = true;
		}

		auto &meshInstance = mesh->instance;
		if (meshInstance != nullptr)
		{
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