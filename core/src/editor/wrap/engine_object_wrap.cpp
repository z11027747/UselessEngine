
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/engine_object_wrap.h"
#include "editor/window.h"
#include "editor/global.h"
#include "context.h"
#include "engine_object.h"

namespace Editor {

	void EngineObjectWrap::Add(Context* context,
		const std::string& name
	) {
		auto eo = std::make_shared<EngineObject>();
		eo->name = name;
		context->allEOs.emplace_back(eo);
		context->allEOMap.emplace(name, eo);
	}

	void EngineObjectWrap::Draw(Context* context,
		std::shared_ptr<EngineObject> eo
	) {
		ImGui::Checkbox("##active", &eo->active);
		ImGui::SameLine();

		ImGui::Text("%s (id: %d)", eo->name.c_str(), eo->id);
		ImGui::SameLine();

		auto isSelected = (Window::selectEO == eo);
		if (ImGui_ButtonWithColor("Select", ImVec4(0.0f, 0.4f, 0.0f, 1.0f), isSelected)) {
			Window::selectEO = eo;
		}
		ImGui::SameLine();

		if (ImGui_ButtonWithColor("Delete", ImVec4(0.4f, 0.0f, 0.0f, 1.0f))) {
			std::cout << "delete" << std::endl;
		}
	}

}