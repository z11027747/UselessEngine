
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/engine_object_wrap.h"
#include "editor/window.h"
#include "editor/global.h"
#include "context.h"
#include "engine_object.h"

namespace Editor {

	static char addEOName[16] = "";

	void Window::DrawHierachy(Context* context) {
		if (ImGui::Begin("Hierarchy", NULL)) {

			auto& io = ImGui::GetIO();
			ImGui::Text("fps: %.1f ms: %.3f", io.Framerate, 1000.0f / io.Framerate);

			ImGui::SeparatorText("EngineObjectList Begin"); {
				auto index = 0;
				auto& eos = context->allEOs;
				for (const auto& eo : eos) {
					ImGui::PushID(index++);
					EngineObjectWrap::Draw(context, eo);
					ImGui::PopID();
				}
			}
			ImGui::SeparatorText("End");

			ImGui::SetNextItemWidth(150.0f);
			ImGui::InputText("##addEOName", addEOName, IM_ARRAYSIZE(addEOName));
			ImGui::SameLine();
			if (ImGui::Button("Add EnginObject")) {
				std::cout << "Add EngineObject Click!" << std::endl;

				EngineObjectWrap::Add(context, std::string(addEOName));
				memset(addEOName, 0, sizeof(addEOName));
			}

			ImGui::End();
		}
	}

}