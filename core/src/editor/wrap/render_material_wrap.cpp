
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
	static const int pipelineNameSize = 4;

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
		auto &info = material->info;
		if (info->pipelineName == Render::Pipeline_Skybox)
		{
			imageNameIndexs.resize(6);
			for (auto i = 0; i < 6; i++)
			{
				imageNameIndexs[i] = FindImageNameIndex(info->imageNames[i]);
			}
		}
		else if (info->pipelineName == Render::Pipeline_Color)
		{
		}
		else if (info->pipelineName == Render::Pipeline_LightModel)
		{
			imageNameIndexs.resize(2);
			for (auto i = 0; i < 2; i++)
			{
				imageNameIndexs[i] = FindImageNameIndex(info->imageNames[i]);
			}
		}
	}

	static void DrawInit(Context *context,
						 std::shared_ptr<Render::Material> material)
	{
		auto &info = material->info;
		auto &pipelineName = info->pipelineName;
		for (auto i = 0; i < pipelineNameSize; i++)
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
	}

	static void DrawImageByIndex(std::shared_ptr<Render::Material> material,
								 const char *showName, int index)
	{
		if (ImGui::Combo(showName, &imageNameIndexs[index],
						 imageNameCStrs.data(), static_cast<int>(imageNameCStrs.size())))
		{
			auto &info = material->info;
			info->imageNames[index] = imageNames[imageNameIndexs[index]];
			info->hasChanged = true;
		}
	}

	template <>
	void ComponentWrap<Render::Material>::Draw(Context *context,
											   std::shared_ptr<Render::Material> material, bool isInit)
	{
		if (isInit)
		{
			DrawInit(context, material);
			return;
		}

		ImGui::Text("InstanceId: %d", material->instance->id);

		auto &info = material->info;
		if (ImGui::Combo("##pipelineNames",
						 &pipelineNameIndex, pipelineNames, IM_ARRAYSIZE(pipelineNames)))
		{
			info->pipelineName = pipelineNames[pipelineNameIndex];
			info->hasChanged = true;

			if (info->pipelineName == Render::Pipeline_Skybox)
			{
				info->imageNames.resize(6, "resource/texture/white.png");
				info->params = {};
			}
			else if (info->pipelineName == Render::Pipeline_Color)
			{
				info->imageNames = {};
				info->params = {glm::vec4(1.0f)};
			}
			else if (info->pipelineName == Render::Pipeline_LightModel)
			{
				info->imageNames = {"resource/texture/white.png", "resource/texture/bump.png"};
				info->params = {glm::vec4(1.0f, 50.0f, 1.0f, 0.0f)};
			}

			FindImageNameIndexsByPipelineName(material);
		}

		if (info->pipelineName == Render::Pipeline_Skybox)
		{
			DrawImageByIndex(material, "+x", 0);
			DrawImageByIndex(material, "-x", 1);
			DrawImageByIndex(material, "+y", 2);
			DrawImageByIndex(material, "-y", 3);
			DrawImageByIndex(material, "+z", 4);
			DrawImageByIndex(material, "-z", 5);
		}
		else if (info->pipelineName == Render::Pipeline_Color)
		{
			auto &params0 = info->params[0];
			ImGui::ColorEdit3("Color", &params0.x);
		}
		else if (info->pipelineName == Render::Pipeline_LightModel)
		{
			DrawImageByIndex(material, "Albedo", 0);
			DrawImageByIndex(material, "NomralMap", 1);

			ImGui::PushItemWidth(150);
			auto &params0 = info->params[0];
			ImGui::DragFloat("DiffuseIntensity", &params0.x, 0.01f);
			ImGui::DragFloat("SpecualrShininess", &params0.y, 0.1f);
			ImGui::DragFloat("SpecularIntensity", &params0.z, 0.01f);
			ImGui::PopItemWidth();
		}

		ImGui::Checkbox("CastShadow", &info->castShadow);
		ImGui::InputInt("RenderQueue", &info->renderQueue);
	}
}