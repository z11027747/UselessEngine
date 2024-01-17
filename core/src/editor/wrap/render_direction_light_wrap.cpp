
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
	static float diffuseIntensity = 0.4f;
	static float specualrShininess = 10.0f;
	static float specularIntensity = 0.5f;

	template <>
	void ComponentWrap<Render::DirectionLight>::Draw(Context *context,
													 std::shared_ptr<Render::DirectionLight> directionLight, bool isFirst)
	{
		if (isFirst)
		{
			FillFloat3(color, directionLight->color);
			diffuseIntensity = directionLight->params.x;
			specualrShininess = directionLight->params.y;
			specularIntensity = directionLight->params.z;
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

		ImGui::PushItemWidth(150);
		if (ImGui::DragFloat("DiffuseIntensity", &diffuseIntensity, 0.01f))
		{
			directionLight->params.x = diffuseIntensity;
		}

		if (ImGui::DragFloat("SpecualrShininess", &specualrShininess, 0.1f))
		{
			directionLight->params.y = specualrShininess;
		}

		if (ImGui::DragFloat("SpecularIntensity", &specularIntensity, 0.01f))
		{
			directionLight->params.z = specularIntensity;
		}
		ImGui::PopItemWidth();

		ImGui::Checkbox("HasShadow", &directionLight->hasShadow);
	}
}