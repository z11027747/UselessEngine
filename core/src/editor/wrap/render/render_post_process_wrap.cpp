
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/post_process/post_process_comp.h"
#include "editor/wrap/component_wrap.h"
#include "context.hpp"

namespace Editor
{
	static bool enableToonMapping = false;
	static bool enableGaussBlur = false;
	static bool enableBloom = false;

	template <>
	void ComponentWrap<Render::PostProcess>::Draw(Context *context,
												  std::shared_ptr<void> component, bool isInit)
	{
		auto postProcess = std::static_pointer_cast<Render::PostProcess>(component);

		auto &toonMappingParams = postProcess->toonMappingParams;
		auto &gaussBlurParams = postProcess->gaussBlurParams;
		auto &bloomParams = postProcess->bloomParams;

		if (isInit)
		{
			enableToonMapping = (toonMappingParams.w == 1.0f);
			enableGaussBlur = (gaussBlurParams.w == 1.0f);
			enableBloom = (bloomParams.w == 1.0f);
			return;
		}

		ImGui::Spacing();
		if (ImGui::Checkbox("Toon Mapping", &enableToonMapping))
		{
			toonMappingParams.w = enableToonMapping ? 1.0f : 0.0f;
		}
		if (enableToonMapping)
		{
			ImGui::SliderFloat("Brightness", &toonMappingParams.x, 0.0f, 3.0f);
			ImGui::SliderFloat("Saturation", &toonMappingParams.y, 0.0f, 3.0f);
			ImGui::SliderFloat("Contrast", &toonMappingParams.z, 0.0f, 3.0f);
		}

		ImGui::Spacing();
		if (ImGui::Checkbox("Gauss Blur", &enableGaussBlur))
		{
			gaussBlurParams.w = enableGaussBlur ? 1.0f : 0.0f;
		}
		if (enableGaussBlur)
		{
			ImGui::SliderFloat("Scale", &gaussBlurParams.x, 0.0f, 10.0f);
			ImGui::SliderFloat("Strength", &gaussBlurParams.y, 0.0f, 10.0f);
			ImGui::SliderFloat("Direction", &gaussBlurParams.z, 0.0f, 1.0f);
		}

		ImGui::Spacing();
		if (ImGui::Checkbox("Bloom", &enableBloom))
		{
			bloomParams.w = enableBloom ? 1.0f : 0.0f;
		}
		if (enableBloom)
		{
			ImGui::SliderFloat("Threshold", &bloomParams.x, 0.0f, 1.0f);
			ImGui::InputFloat("Intensity", &bloomParams.y);
		}
	}
}