
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/post_process/post_process_comp.h"
#include "editor/wrap/component_wrap.h"
#include "context.h"

namespace Editor
{
	template <>
	void ComponentWrap<Render::PostProcess>::Draw(Context *context,
												  std::shared_ptr<Render::PostProcess> postProcess, bool isInit)
	{

		if (isInit)
		{
			return;
		}

		ImGui::Checkbox("Enabled", &postProcess->enabled);

		ImGui::Spacing();
		ImGui::Text("Toon Mapping");
		auto &toonMappingParams = postProcess->toonMappingParams;
		ImGui::SliderFloat("Brightness", &toonMappingParams.x, 0.0f, 3.0f);
		ImGui::SliderFloat("Saturation", &toonMappingParams.y, 0.0f, 3.0f);
		ImGui::SliderFloat("Contrast", &toonMappingParams.z, 0.0f, 3.0f);

		ImGui::Spacing();
		ImGui::Text("Bloom");
		auto &bloomParams = postProcess->bloomParams;
		ImGui::SliderFloat("Scale", &bloomParams.x, 0.0f, 10.0f);
		ImGui::SliderFloat("Threshold", &bloomParams.y, 0.0f, 1.0f);
		ImGui::InputFloat("Intensity", &bloomParams.z);
	}
}