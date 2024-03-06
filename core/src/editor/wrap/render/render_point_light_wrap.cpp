
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/light/light_comp.h"
#include "editor/wrap/component_wrap.h"
#include "context.hpp"

namespace Editor
{
	template <>
	void ComponentWrap<Render::PointLight>::Draw(Context *context,
												 std::shared_ptr<void> component, bool isInit)
	{
		auto pointLight = std::static_pointer_cast<Render::PointLight>(component);

		if (isInit)
		{
			return;
		}

		ImGui::ColorEdit3("Color", &pointLight->color.r);
		ImGui::InputFloat("Intensity", &pointLight->color.a);

		ImGui::Spacing();
		ImGui::InputFloat("Constant", &pointLight->clq.x);
		ImGui::InputFloat("Linear", &pointLight->clq.y);
		ImGui::InputFloat("Quadratic", &pointLight->clq.z, 0.0f, 0.0f, "%.4f");

		// ImGui::Checkbox("HasShadow", &pointLight->hasShadow);
	}
}