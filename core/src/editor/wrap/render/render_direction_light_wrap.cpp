
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/light/light_comp.h"
#include "editor/wrap/component_wrap.h"
#include "editor/imgui_logic.h"
#include "context.h"

namespace Editor
{
	template <>
	void ComponentWrap<Render::DirectionLight>::Draw(Context *context,
													 std::shared_ptr<Render::DirectionLight> directionLight, bool isInit)
	{
		if (isInit)
		{
			return;
		}

		if (ImGui::ColorEdit3("Ambient", &directionLight->ambient.x))
		{
		}
		if (ImGui::ColorEdit3("Color", &directionLight->color.x))
		{
		}

		ImGui::Checkbox("HasShadow", &directionLight->hasShadow);
	}
}