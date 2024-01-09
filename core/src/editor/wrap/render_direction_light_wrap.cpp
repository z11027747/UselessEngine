
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/component_wrap.h"
#include "context.h"

namespace Editor
{
	static float color[3] = {1.0f, 0.9568627f, 0.8392157f};
	static float diffuseIntensity = 0.4f;
	static float specualrShininess = 10.0f;
	static float specularIntensity = 0.5f;

	void ComponentWrap<Render::DirectionLight>::Draw(Context *context,
													 std::shared_ptr<Render::DirectionLight> directionLight, bool isFirst)
	{
		if (isFirst)
		{
			color[0] = directionLight->color.r;
			color[1] = directionLight->color.g;
			color[2] = directionLight->color.b;
			diffuseIntensity = directionLight->params.x;
			specualrShininess = directionLight->params.y;
			specularIntensity = directionLight->params.z;
		}

		if (ImGui::ColorEdit3("Color", color))
		{
			directionLight->color.r = color[0];
			directionLight->color.g = color[1];
			directionLight->color.b = color[2];
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