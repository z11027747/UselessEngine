
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include <string>
#include <typeindex>
#include "logic/move/move_logic.h"
#include "editor/wrap/component_wrap.h"
#include "editor/system.h"
#include "editor/window.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
	void DrawComponent(Context *context,
					   std::type_index typeId, std::shared_ptr<void> component,
					   bool isFirst)
	{
		if (typeId == typeid(Logic::Transform))
		{
			auto transform = std::static_pointer_cast<Logic::Transform>(component);
			ComponentWrap<Logic::Transform>::Draw(context,
												  transform, isFirst);
		}
		else if (typeId == typeid(Logic::Camera))
		{
			auto camera = std::static_pointer_cast<Logic::Camera>(component);
			ComponentWrap<Logic::Camera>::Draw(context,
											   camera, isFirst);
		}
		else if (typeId == typeid(Render::DirectionLight))
		{
			auto directionLight = std::static_pointer_cast<Render::DirectionLight>(component);
			ComponentWrap<Render::DirectionLight>::Draw(context,
														directionLight, isFirst);
		}
		else if (typeId == typeid(Render::Unit))
		{
			auto unit = std::static_pointer_cast<Render::Unit>(component);
			ComponentWrap<Render::Unit>::Draw(context,
											  unit, isFirst);
		}
	}

	std::shared_ptr<EngineObject> Window::selectEO = nullptr;

	void Window::SetSelectEO(Context *context, std::shared_ptr<EngineObject> eo)
	{
		selectEO = eo;

		auto axisEO = context->GetEO(Name_Axis);
		axisEO->active = true;

		Logic::MoveLogic::BeginFollow(context,
									  axisEO,
									  eo, glm::vec3(0.0f));

		auto &componentMap = selectEO->componentMap;
		for (const auto &kv : componentMap)
		{
			auto typeId = kv.first;
			auto &component = kv.second;
			DrawComponent(context, typeId, component, true);
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

				auto id = 0;
				auto &componentMap = selectEO->componentMap;
				for (const auto &kv : componentMap)
				{
					auto typeId = kv.first;
					ImGui::PushID(id++);
					ImGui::SetNextItemOpen(true);
					if (ImGui::TreeNode("Comp: &s", typeId.name()))
					{
						auto &component = kv.second;
						DrawComponent(context, typeId, component, false);
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