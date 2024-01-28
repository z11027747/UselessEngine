
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/mesh/mesh_comp.h"
#include "logic/move/move_comp.h"
#include "logic/move/move_logic.h"
#include "editor/wrap/component_wrap.h"
#include "editor/wrap/component_wrap_mapping.h"
#include "editor/json/component_json.h"
#include "editor/window.h"
#include "common/reflection/type.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
	static std::shared_ptr<EngineObject> selectEO = nullptr;

	std::shared_ptr<EngineObject> Window::GetSelectEO()
	{
		return selectEO;
	}

	void Window::SetSelectEO(Context *context, std::shared_ptr<EngineObject> eo)
	{
		selectEO = eo;

		auto validEO = (eo->name != Define::EOName::MainCamera &&
						eo->name != Define::EOName::Skybox &&
						eo->name != Define::EOName::AxisX &&
						eo->name != Define::EOName::AxisY &&
						eo->name != Define::EOName::AxisZ);

		auto axisEO = context->GetEO(Define::EOName::Axis);
		axisEO->active = validEO;

		if (validEO)
			Logic::MoveLogic::BeginFollow(context,
										  axisEO,
										  eo, glm::vec3(0.0f));

		auto &componentMap = selectEO->componentMap;
		for (const auto &kv : componentMap)
		{
			auto type = kv.first;
			auto &component = kv.second;
			DrawWrap(context, type, component, true);
		}
	}

	static int addComponentTypeIndex = -1;
	static const char *addComponentTypes[] = {
		// logic
		Logic::Camera::type.c_str(), Logic::Transform::type.c_str(),
		Logic::MoveFowrard::type.c_str(), Logic::MoveFollow::type.c_str(),
		Logic::RotateAround::type.c_str(),
		// render
		Render::DirectionLight::type.c_str(), Render::PointLight::type.c_str(), Render::SpotLight::type.c_str(),
		Render::Material::type.c_str(),
		Render::Mesh::type.c_str()};

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
					auto type = kv.first;
					auto &component = kv.second;

					if (!HasWrap(context, type))
						continue;

					ImGui::PushID(type.c_str());
					ImGui::SetNextItemOpen(true);
					if (ImGui::TreeNode("Comp: &s", type.c_str()))
					{
						DrawWrap(context, type, component, false);

						ImGui::TreePop();
					}
					ImGui::PopID();
				}

				ImGui::SeparatorText("End");

				ImGui::SetNextItemWidth(200.0f);
				ImGui::Combo("##addComponentTypes", &addComponentTypeIndex, addComponentTypes, IM_ARRAYSIZE(addComponentTypes));
				ImGui::SameLine();

				if (ImGui::Button("Add Component"))
				{
					// std::cout << "Add Component Click!" << std::endl;
					auto componentType = std::string(addComponentTypes[addComponentTypeIndex]);
					auto component = Common::Type::Create(componentType);
					context->AddComponent(selectEO,
										  componentType, component);

					// for refresh
					SetSelectEO(context, selectEO);
				}
			}
		}

		ImGui::End();
	}
}