
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/material/material_comp.h"
#include "editor/wrap/component_wrap.h"
#include "editor/window.h"
#include "editor/test_logic.h"
#include "context.h"

namespace Editor
{
	static int pipelineNameIndex = -1;
	static const char *pipelineNames[] = {Render::Pipeline_Skybox.c_str(),
										  Render::Pipeline_Shadow.c_str(),
										  Render::Pipeline_LightModel.c_str(),
										  Render::Pipeline_Color.c_str()};

	static int imageNameIndex = -1;
	static std::vector<std::string> imageNames = {};
	static std::vector<const char *> imageNameCStrs = {};

	template <>
	void ComponentWrap<Render::Material>::Draw(Context *context,
											   std::shared_ptr<Render::Material> material, bool isFirst)
	{
		if (isFirst)
		{
			auto &pipelineName = material->pipelineName;
			for (auto i = 0; i < 4; i++)
			{
				if (strcmp(pipelineNames[i], pipelineName.c_str()) == 0)
				{
					pipelineNameIndex = i;
					break;
				}
			}

			imageNameIndex = -1;
			imageNames.clear();
			Window::GetDirectoryFiles("resource/texture", imageNames);

			auto &imageName = material->imageNames[0];
			auto it = std::find(imageNames.begin(), imageNames.end(), imageName);
			if (it != imageNames.end())
			{
				auto index = std::distance(imageNames.begin(), it);
				imageNameIndex = static_cast<int>(index);
			}

			imageNameCStrs.clear();
			for (auto &imageName : imageNames)
			{
				imageNameCStrs.push_back(imageName.c_str());
			}

			return;
		}

		if (ImGui::Combo("##pipelineNames",
						 &pipelineNameIndex, pipelineNames, IM_ARRAYSIZE(pipelineNames)))
		{
		}

		if (ImGui::Combo("ImageName", &imageNameIndex,
						 imageNameCStrs.data(), static_cast<int>(imageNameCStrs.size())))
		{
			material->imageNames[0] = imageNames[imageNameIndex];
			material->hasChanged = true;
		}

		ImGui::Checkbox("CastShadow", &material->castShadow);
	}
}