
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

	static std::vector<int> imageNameIndexs = {};
	static std::vector<std::string> imageNames = {};
	static std::vector<const char *> imageNameCStrs = {};

	static int FindImageNameIndex(std::string &imageName)
	{
		auto it = std::find(imageNames.begin(), imageNames.end(), imageName);
		if (it != imageNames.end())
		{
			auto index = std::distance(imageNames.begin(), it);
			return static_cast<int>(index);
		}

		return -1;
	}

	static void FindImageNameIndexsByPipelineName(std::shared_ptr<Render::Material> material)
	{
		if (material->pipelineName == Render::Pipeline_Color)
		{
		}
		else if (material->pipelineName == Render::Pipeline_LightModel)
		{
			imageNameIndexs.resize(3);
			for (auto i = 0; i < 3; i++)
			{
				imageNameIndexs[i] = FindImageNameIndex(material->imageNames[i]);
			}
		}
	}

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

			imageNameIndexs.clear();
			imageNames.clear();
			Window::GetDirectoryFiles("resource/texture", imageNames);

			imageNameCStrs.clear();
			for (auto &imageName : imageNames)
			{
				imageNameCStrs.push_back(imageName.c_str());
			}

			FindImageNameIndexsByPipelineName(material);
			return;
		}

		if (ImGui::Combo("##pipelineNames",
						 &pipelineNameIndex, pipelineNames, IM_ARRAYSIZE(pipelineNames)))
		{
			material->pipelineName = pipelineNames[pipelineNameIndex];
			material->hasChanged = true;

			auto &imageNames = material->imageNames;
			if (material->pipelineName == Render::Pipeline_Color)
			{
				imageNames = {};
			}
			else if (material->pipelineName == Render::Pipeline_LightModel)
			{
				imageNames = {"resource/texture/white.png",
							  "resource/texture/white.png",
							  "resource/texture/white.png"};
			}
			FindImageNameIndexsByPipelineName(material);
		}

		if (material->pipelineName == Render::Pipeline_Color)
		{
		}
		else if (material->pipelineName == Render::Pipeline_LightModel)
		{
			if (ImGui::Combo("Albedo", &imageNameIndexs[0],
							 imageNameCStrs.data(), static_cast<int>(imageNameCStrs.size())))
			{
				material->imageNames[1] = imageNames[imageNameIndexs[0]];
				material->hasChanged = true;
			}

			if (ImGui::Combo("Specular", &imageNameIndexs[1],
							 imageNameCStrs.data(), static_cast<int>(imageNameCStrs.size())))
			{
				material->imageNames[2] = imageNames[imageNameIndexs[2]];
				material->hasChanged = true;
			}

			if (ImGui::Combo("NomralMap", &imageNameIndexs[2],
							 imageNameCStrs.data(), static_cast<int>(imageNameCStrs.size())))
			{
				material->imageNames[3] = imageNames[imageNameIndexs[2]];
				material->hasChanged = true;
			}
		}

		ImGui::Checkbox("CastShadow", &material->castShadow);
	}
}