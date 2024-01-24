
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/light/light_comp.h"
#include "editor/wrap/component_wrap.h"
#include "editor/test_logic.h"
#include "context.h"

namespace Editor
{
	static float color[3] = {1.0f, 0.9568627f, 0.8392157f};

	template <>
	void ComponentWrap<Render::PointLight>::Draw(Context *context,
												 std::shared_ptr<Render::PointLight> pointLight, bool isInit)
	{
		if (isInit)
		{
			FillFloat3(color, pointLight->color);
			return;
		}

		if (ImGui::ColorEdit3("Color", color))
		{
			FillGlmVec3(pointLight->color, color);
		}

		ImGui::InputFloat("Constant", &pointLight->clq.x);
		ImGui::InputFloat("Linear", &pointLight->clq.y);
		ImGui::InputFloat("Quadratic", &pointLight->clq.z);

		ImGui::Checkbox("HasShadow", &pointLight->hasShadow);
	}
}