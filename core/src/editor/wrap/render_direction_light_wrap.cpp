
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/light/light_comp.h"
#include "editor/wrap/component_wrap.h"
#include "editor/test_logic.h"
#include "context.h"

namespace Editor
{
	static float ambient[3] = {0.1f, 0.1f, 0.1f};
	static float color[3] = {1.0f, 0.9568627f, 0.8392157f};

	template <>
	void ComponentWrap<Render::DirectionLight>::Draw(Context *context,
													 std::shared_ptr<Render::DirectionLight> directionLight, bool isInit)
	{
		if (isInit)
		{
			FillFloat3(color, directionLight->color);
			FillFloat3(ambient, directionLight->ambient);
			return;
		}

		if (ImGui::ColorEdit3("Ambient", ambient))
		{
			FillGlmVec3(directionLight->ambient, ambient);
		}

		if (ImGui::ColorEdit3("Color", color))
		{
			FillGlmVec3(directionLight->color, color);
		}

		ImGui::Checkbox("HasShadow", &directionLight->hasShadow);
	}
}