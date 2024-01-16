
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/mesh/mesh_comp.h"
#include "logic/move/move_logic.h"
#include "editor/wrap/component_wrap.h"
#include "editor/wrap/component_wrap_mapping.h"
#include "editor/json/component_json.h"
#include "editor/json/component_json_mapping.h"
#include "editor/window.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
	std::shared_ptr<EngineObject> Window::selectEO = nullptr;

	void Window::SetSelectEO(Context *context, std::shared_ptr<EngineObject> eo)
	{
		selectEO = eo;

		if (eo->name != Name_MainCamera && eo->name != Name_Skybox &&
			eo->name != Name_AxisX && eo->name != Name_AxisY && eo->name != Name_AxisZ)
		{
			auto axisEO = context->GetEO(Name_Axis);
			axisEO->active = true;

			// auto mesh = eo->GetComponent<Render::Mesh>();
			// auto &meshCenter = mesh->bound.center;

			Logic::MoveLogic::BeginFollow(context,
										  axisEO,
										  eo, glm::vec3(0.0f));
		}

		auto &componentMap = selectEO->componentMap;
		for (const auto &kv : componentMap)
		{
			auto typeId = kv.first;
			auto &component = kv.second;
			DrawWrap(context, typeId, component, true);
		}
	}

	static int addComponentTypeIndex = 0;
	const char *addComponentTypes[] = {"",
									   Type_Logic_Camera.c_str(),
									   Type_Logic_Transform.c_str(),
									   Type_Render_DirectionLight.c_str(),
									   Type_Render_Material.c_str(),
									   Type_Render_Mesh.c_str()};

	void Window::DrawInspector(Context *context)
	{
		if (ImGui::Begin("Inspector", NULL))
		{
			if (selectEO == nullptr)
			{
				ImGui::Text("Please Select an EngineObject");
			}
			else
			{
				ImGui::Text("Select EngineObject: %s", selectEO->name.data());
				ImGui::SeparatorText("ComponentList Begin");

				auto &componentMap = selectEO->componentMap;
				for (const auto &kv : componentMap)
				{
					auto typeId = kv.first;
					auto &component = kv.second;

					if (!HasWrap(context, typeId))
						continue;

					auto typeName = typeId.name();
					ImGui::PushID(typeName);
					ImGui::SetNextItemOpen(true);
					if (ImGui::TreeNode("Comp: &s", typeName))
					{
						DrawWrap(context, typeId, component, false);

						ImGui::TreePop();
					}
					ImGui::PopID();
				}

				ImGui::SeparatorText("End");

				ImGui::SetNextItemWidth(150.0f);
				ImGui::Combo("##addComponentTypes", &addComponentTypeIndex, addComponentTypes, IM_ARRAYSIZE(addComponentTypes));
				ImGui::SameLine();

				if (ImGui::Button("Add Component"))
				{
					// std::cout << "Add Component Click!" << std::endl;
					auto componentType = std::string(addComponentTypes[addComponentTypeIndex]);

					EOAddDefault(context, selectEO, componentType);
				}
			}
		}

		ImGui::End();
	}
}