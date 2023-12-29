
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/logic_component_wrap.h"
#include "editor/global.h"
#include "context.h"

namespace Editor {

	void LogicComponentWrap::DrawTransform(Context* context,
		std::shared_ptr<Logic::Transform> transform
	) {
		ImGui::PushID(0); {
			ImGui_Drag_GlmVec3("Position", transform->position);
		}
		ImGui::PopID();

		ImGui::PushID(1); {
			ImGui_Drag_GlmVec3("EulerAngels", transform->eulerAngles, 60, 0.03f);
		}
		ImGui::PopID();

		ImGui::PushID(2); {
			ImGui_Drag_GlmVec3("Scale", transform->scale);
		}
		ImGui::PopID();
	}

}