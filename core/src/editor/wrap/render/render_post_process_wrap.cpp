
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/post_process/post_process_comp.h"
#include "editor/wrap/component_wrap.h"
#include "context.h"

namespace Editor
{
	static bool enableBloom = false;

	template <>
	void ComponentWrap<Render::PostProcess>::Draw(Context *context,
												  std::shared_ptr<Render::PostProcess> postProcess, bool isInit)
	{
		auto &bloomParams = postProcess->bloomParams;

		if (isInit)
		{
			enableBloom = (bloomParams.x == 1.0f);
			return;
		}

		ImGui::Checkbox("Enabled", &postProcess->enabled);

		ImGui::Spacing();
		if (ImGui::Checkbox("Enable Bloom", &enableBloom))
		{
			bloomParams.x = enableBloom ? 1.0f : 0.0f;
		}
		if (enableBloom)
		{
			ImGui::DragFloat("Scale", &bloomParams.y, 0.1f);
			ImGui::DragFloat("Threshold", &bloomParams.z, 0.01f, 0.0f, 1.0f);
			ImGui::InputFloat("Intensity", &bloomParams.w);
		}
	}
}