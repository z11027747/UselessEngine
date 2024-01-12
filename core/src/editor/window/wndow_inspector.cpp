
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include <string>
#include <typeindex>
#include <functional>
#include "logic/transform/transform_logic.h"
#include "logic/move/move_logic.h"
#include "editor/wrap/component_wrap.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "editor/window.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
	template <typename T>
	static void DrawComponent(Context *context, std::shared_ptr<void> component, bool isFirst)
	{
		auto concreteComponent = std::static_pointer_cast<T>(component);
		ComponentWrap<T>::Draw(context, concreteComponent, isFirst);
	}

	static std::unordered_map<std::type_index, std::function<void(Context *, std::shared_ptr<void>, bool)>> drawComponentFuncMap{
		{typeid(Logic::Transform), DrawComponent<Logic::Transform>},
		{typeid(Logic::Camera), DrawComponent<Logic::Camera>},
		{typeid(Render::DirectionLight), DrawComponent<Render::DirectionLight>},
		{typeid(Render::Mesh), DrawComponent<Render::Mesh>},
		{typeid(Render::Material), DrawComponent<Render::Material>},
	};

	std::shared_ptr<EngineObject> Window::selectEO = nullptr;

	void Window::SetSelectEO(Context *context, std::shared_ptr<EngineObject> eo)
	{
		selectEO = eo;

		if (eo->name != Name_MainCamera && eo->name != Name_Skybox &&
			eo->name != Name_AxisX && eo->name != Name_AxisY && eo->name != Name_AxisZ)
		{
			auto axisEO = context->GetEO(Name_Axis);
			axisEO->active = true;

			Logic::MoveLogic::BeginFollow(context,
										  axisEO,
										  eo, glm::vec3(0.0f));
		}

		auto &componentMap = selectEO->componentMap;
		for (const auto &kv : componentMap)
		{
			auto typeId = kv.first;

			auto it = drawComponentFuncMap.find(typeId);
			if (it == drawComponentFuncMap.end())
				continue;

			auto &component = kv.second;
			drawComponentFuncMap[typeId](context, component, true);
		}
	}

	static char addCompName[16] = "";

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

					auto it = drawComponentFuncMap.find(typeId);
					if (it == drawComponentFuncMap.end())
						continue;

					auto typeName = typeId.name();
					ImGui::PushID(typeName);
					ImGui::SetNextItemOpen(true);
					if (ImGui::TreeNode("Comp: &s", typeName))
					{
						auto &component = kv.second;
						drawComponentFuncMap[typeId](context, component, false);

						ImGui::TreePop();
					}
					ImGui::PopID();
				}

				ImGui::SeparatorText("End");

				ImGui::SetNextItemWidth(150.0f);
				ImGui::InputText("##addCompName", addCompName, IM_ARRAYSIZE(addCompName));
				ImGui::SameLine();

				if (ImGui::Button("Add Component"))
				{
					std::cout << "Add Component Click!" << std::endl;
					// memset(addCompName, 0, sizeof(addCompName));
				}
			}
		}

		ImGui::End();
	}
}