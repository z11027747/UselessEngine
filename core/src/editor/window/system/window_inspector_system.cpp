
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/post_process/post_process_comp.h"
#include "logic/transform/transform_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/move/move_comp.h"
#include "logic/rotate/rotate_comp.h"
#include "editor/wrap/component_wrap.h"
#include "editor/window/window_system.hpp"
#include "editor/window/window_logic.hpp"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
	static int addComponentTypeIndex = -1;
	static const char *addComponentTypes[] =
		{
			// logic
			Logic::Camera::type.c_str(),
			Logic::Transform::type.c_str(),
			Logic::MoveFowrard::type.c_str(),
			Logic::MoveFollow::type.c_str(),
			Logic::RotateAround::type.c_str(),
			// render
			Render::DirectionLight::type.c_str(),
			Render::PointLight::type.c_str(),
			Render::SpotLight::type.c_str(),
			Render::Material::type.c_str(),
			Render::Mesh::type.c_str(),
			Render::PostProcess::type.c_str()};

	void WindowInspectorSystem::Update(Context *context)
	{
		if (ImGui::Begin("Inspector", NULL))
		{
			auto selectEO = WindowLogic::GetSelectEO();
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

					if (!ComponentWrapMapping::Has(type))
						continue;

					ImGui::PushID(type.c_str());
					ImGui::SetNextItemOpen(true);
					if (ImGui::TreeNode("Comp: &s", type.c_str()))
					{
						ComponentWrapMapping::Draw(context, type, component, false);

						ImGui::TreePop();
					}
					ImGui::PopID();
				}

				ImGui::SeparatorText("End");

				ImGui::SetNextItemWidth(200.0f);
				ImGui::Combo("##addComponentTypes",
							 &addComponentTypeIndex, addComponentTypes, IM_ARRAYSIZE(addComponentTypes));
				ImGui::SameLine();

				if (ImGui::Button("Add Component"))
				{
					// std::cout << "Add Component Click!" << std::endl;
					auto componentType = std::string(addComponentTypes[addComponentTypeIndex]);
					auto component = EngineComponent::Create(componentType);
					context->AddComponent(selectEO,
										  componentType, component);

					// for refresh
					WindowLogic::SetSelectEO(context, selectEO);
				}
			}
		}

		ImGui::End();
	}
}