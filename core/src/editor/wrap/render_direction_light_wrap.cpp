
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include "editor/wrap/render_component_wrap.h"
#include "editor/global.h"
#include "context.h"

namespace Editor
{
	void RenderComponentWrap::DrawDirectionLight(Context *context,
												 std::shared_ptr<Render::DirectionLight> directionLight)
	{
		static float color[3] = {1.0f, 0.9568627f, 0.8392157f};

		if (ImGui::ColorEdit3("Color", color))
		{
			directionLight->color.r = color[0];
			directionLight->color.g = color[1];
			directionLight->color.b = color[2];
		}

		ImGui::PushItemWidth(150);
		static float diffuseIntensity = 0.4f;
		if (ImGui::DragFloat("DiffuseIntensity", &diffuseIntensity, 0.01f))
		{
			directionLight->params.x = diffuseIntensity;
		}

		static float specualrShininess = 10.0f;
		if (ImGui::DragFloat("SpecualrShininess", &specualrShininess, 0.1f))
		{
			directionLight->params.y = specualrShininess;
		}

		static float specularIntensity = 0.5f;
		if (ImGui::DragFloat("SpecularIntensity", &specularIntensity, 0.01f))
		{
			directionLight->params.z = specularIntensity;
		}
		ImGui::PopItemWidth();
	}
}