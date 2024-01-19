
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
		if (material->pipelineName == Render::Pipeline_Skybox)
		{
			imageNameIndexs.resize(6);
			for (auto i = 0; i < 6; i++)
			{
				imageNameIndexs[i] = FindImageNameIndex(material->imageNames[i]);
			}
		}
		else if (material->pipelineName == Render::Pipeline_Color)
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

	static void DrawImageByIndex(std::shared_ptr<Render::Material> material,
								 const char *showName, int index)
	{
		if (ImGui::Combo(showName, &imageNameIndexs[index],
						 imageNameCStrs.data(), static_cast<int>(imageNameCStrs.size())))
		{
			material->imageNames[index] = imageNames[imageNameIndexs[index]];
			material->hasChanged = true;
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

			if (material->pipelineName == Render::Pipeline_Skybox)
			{
				material->imageNames.resize(6, "resource/texture/white.png");
				material->params = {};
			}
			else if (material->pipelineName == Render::Pipeline_Color)
			{
				material->imageNames = {};
				material->params = {};
			}
			else if (material->pipelineName == Render::Pipeline_LightModel)
			{
				material->imageNames.resize(3, "resource/texture/white.png");
				material->params = {glm::vec4(1.0f, 100.0f, 1.0f, 0.0f)};
			}
			FindImageNameIndexsByPipelineName(material);
		}

		if (material->pipelineName == Render::Pipeline_Skybox)
		{
			DrawImageByIndex(material, "+x", 0);
			DrawImageByIndex(material, "-x", 1);
			DrawImageByIndex(material, "+y", 2);
			DrawImageByIndex(material, "-y", 3);
			DrawImageByIndex(material, "+z", 4);
			DrawImageByIndex(material, "-z", 5);
		}
		else if (material->pipelineName == Render::Pipeline_Color)
		{
		}
		else if (material->pipelineName == Render::Pipeline_LightModel)
		{
			DrawImageByIndex(material, "Albedo", 0);
			DrawImageByIndex(material, "Specular", 1);
			DrawImageByIndex(material, "NomralMap", 2);

			ImGui::PushItemWidth(150);
			ImGui::DragFloat("DiffuseIntensity", &material->params[0].x, 0.01f);
			ImGui::DragFloat("SpecualrShininess", &material->params[0].y, 0.1f);
			ImGui::DragFloat("SpecularIntensity", &material->params[0].z, 0.01f);
			ImGui::PopItemWidth();
		}

		ImGui::Checkbox("CastShadow", &material->castShadow);
	}
}