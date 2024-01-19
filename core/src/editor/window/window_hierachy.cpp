
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "editor/window.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
	static char addEOName[16] = "";

	void DrawEO(Context *context,
				std::shared_ptr<EngineObject> eo, int index)
	{
		ImGui::PushID(index);

		ImGui::Checkbox("##active", &eo->active);
		ImGui::SameLine();

		if (ImGui::Selectable(eo->name.data(), Window::GetSelectEO() == eo))
		{
			Window::SetSelectEO(context, eo);
		}

		ImGui::PopID();
	}

	void Window::DrawHierachy(Context *context)
	{
		if (ImGui::Begin("Hierarchy", NULL))
		{
			auto &io = ImGui::GetIO();
			ImGui::Text("fps: %.1f ms: %.3f", io.Framerate, 1000.0f / io.Framerate);

			ImGui::SeparatorText("EngineObjectList Begin");

			auto &eos = context->allEOs;
			auto eoSize = static_cast<int>(eos.size());
			for (auto i = 0; i < eoSize; i++)
			{
				auto &eo = eos[i];
				if (eo->hideInHierarchy)
					continue;

				auto transform = eo->GetComponent<Logic::Transform>();

				auto &parentEOName = transform->parentEOName;
				auto &parentEO = context->GetEO(parentEOName);
				if (parentEO != nullptr)
					continue;

				DrawEO(context, eo, i);

				auto &childEONames = transform->childEONames;
				auto childEOCount = static_cast<int>(childEONames.size());
				if (childEOCount > 0)
				{
					ImGui::Indent();
					auto childIBegin = i * 100;
					for (auto childI = childIBegin; childI < childIBegin + childEOCount; childI++)
					{
						auto &childEOName = childEONames[childI - childIBegin];
						auto &childEO = context->GetEO(childEOName);
						DrawEO(context, childEO, childI);
					}
					ImGui::Unindent();
				}
			}

			if (ImGui::BeginPopupContextItem("Hierarchy"))
			{
				if (ImGui::Button("Delete"))
				{
					std::cout << "Delete Click!" << std::endl;
				}
				ImGui::EndPopup();
			}

			ImGui::SeparatorText("End");

			ImGui::SetNextItemWidth(150.0f);
			ImGui::InputText("##addEOName", addEOName, IM_ARRAYSIZE(addEOName));
			ImGui::SameLine();

			if (ImGui::Button("Add EnginObject"))
			{
				// std::cout << "Add EngineObject Click!" << std::endl;
				auto name = std::string(addEOName);
				memset(addEOName, 0, sizeof(addEOName));

				auto eo = std::make_shared<EngineObject>();
				eo->name = name;

				Logic::TransformLogic::Add(eo,
										   glm::vec3(0.0f, 0.0f, 0.0f),
										   glm::vec3(0.0f, 0.0f, 0.0f),
										   glm::vec3(1.0f, 1.0f, 1.0f));

				context->AddEO(eo);
			}
		}
		ImGui::End();
	}
}