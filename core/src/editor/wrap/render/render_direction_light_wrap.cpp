
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/light/light_comp.h"
#include "editor/wrap/component_wrap.h"
#include "context.hpp"

namespace Editor
{
	template <>
	void ComponentWrap<Render::DirectionLight>::Draw(Context *context,
													 std::shared_ptr<void> component, bool isInit)
	{
		auto directionLight = std::static_pointer_cast<Render::DirectionLight>(component);

		if (isInit)
		{
			return;
		}

		ImGui::ColorEdit3("Ambient", &directionLight->ambient.x);
		ImGui::ColorEdit3("Color", &directionLight->color.x);

		ImGui::Checkbox("HasShadow", &directionLight->hasShadow);
	}
}