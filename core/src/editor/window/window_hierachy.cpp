
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/window.h"
#include "editor/global.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
	static char addEOName[16] = "";
	static int selectEOIndex = -1;

	void Window::DrawHierachy(Context *context)
	{
		if (ImGui::Begin("Hierarchy", NULL))
		{
			auto &io = ImGui::GetIO();
			ImGui::Text("fps: %.1f ms: %.3f", io.Framerate, 1000.0f / io.Framerate);

			ImGui::SeparatorText("EngineObjectList Begin");
			{
				auto &eos = context->allEOs;
				auto eoSize = eos.size();
				for (auto i = 0; i < eoSize; i++)
				{
					ImGui::PushID(i);

					auto &eo = eos[i];

					ImGui::Checkbox("##active", &eo->active);
					ImGui::SameLine();

					if (ImGui::Selectable(eo->name.data(), selectEOIndex == i))
					{
						selectEOIndex = i;
						Window::selectEO = eo;
					}

					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::Button("Delete"))
						{
							std::cout << "Delete Click!" << std::endl;
						}
						ImGui::EndPopup();
					}

					ImGui::PopID();
				}
			}
			ImGui::SeparatorText("End");

			ImGui::SetNextItemWidth(150.0f);
			ImGui::InputText("##addEOName", addEOName, IM_ARRAYSIZE(addEOName));
			ImGui::SameLine();

			if (ImGui::Button("Add EnginObject"))
			{
				std::cout << "Add EngineObject Click!" << std::endl;

				auto name = std::string(addEOName);
				memset(addEOName, 0, sizeof(addEOName));

				auto eo = std::make_shared<EngineObject>();
				eo->name = name;
				context->allEOs.emplace_back(eo);
				context->allEOMap.emplace(name, eo);
			}
		}
		ImGui::End();
	}
}