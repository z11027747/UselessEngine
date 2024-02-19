
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/light/light_comp.h"
#include "editor/wrap/component_wrap.h"
#include "context.hpp"

namespace Editor
{
	template <>
	void ComponentWrap<Render::SpotLight>::Draw(Context *context,
												std::shared_ptr<void> component, bool isInit)
	{
		auto spotLight = std::static_pointer_cast<Render::SpotLight>(component);

		if (isInit)
		{
			return;
		}

		ImGui::ColorEdit3("Color", &spotLight->color.x);

		if (ImGui::InputFloat("CutOff", &spotLight->cutOff.z))
		{
			spotLight->cutOff.x = glm::cos(glm::radians(spotLight->cutOff.z));
		}
		if (ImGui::InputFloat("OuterCutOff", &spotLight->cutOff.w))
		{
			spotLight->cutOff.y = glm::cos(glm::radians(spotLight->cutOff.w));
		}

		// ImGui::Text("For Debug");
		// ImGui::InputFloat("CutOff_X", &spotLight->cutOff.x);
		// ImGui::InputFloat("CutOff_Y", &spotLight->cutOff.y);

		// ImGui::Checkbox("HasShadow", &spotLight->hasShadow);
	}
}