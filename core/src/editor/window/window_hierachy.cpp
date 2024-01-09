
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "logic/transform/transform_comp.h"
#include "editor/window.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
	static char addEOName[16] = "";
	static int selectEOIndex = -1;

	void DrawEO(Context *context,
				std::shared_ptr<EngineObject> eo, int index)
	{
		ImGui::PushID(index);

		ImGui::Checkbox("##active", &eo->active);
		ImGui::SameLine();

		if (ImGui::Selectable(eo->name.data(), selectEOIndex == index))
		{
			selectEOIndex = index;
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

				auto &parentEO = transform->parentEO;
				if (parentEO != nullptr)
					continue;

				DrawEO(context, eo, i);

				auto &childEOs = transform->childEOs;
				auto childEOCount = static_cast<int>(childEOs.size());
				if (childEOCount > 0)
				{
					ImGui::Indent();
					auto childIBegin = i * 100;
					for (auto childI = childIBegin; childI < childIBegin + childEOCount; childI++)
					{
						DrawEO(context, childEOs[childI - childIBegin], childI);
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
				context->AddEO(eo);
			}
		}
		ImGui::End();
	}
}