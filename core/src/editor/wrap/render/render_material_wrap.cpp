
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/material/material_comp.h"
#include "define.hpp"
#include "editor/wrap/component_wrap.h"
#include "editor/window/window_logic.hpp"
#include "context.hpp"

namespace Editor
{
	static int pipelineNameIndex = -1;

	static const int pipelineNameSize = 10;
	static const char *pipelineNames[] =
		{
			Define::Pipeline::Skybox.c_str(),
			Define::Pipeline::Shadow.c_str(),
			Define::Pipeline::LightModel.c_str(),
			Define::Pipeline::PBR_Simple.c_str(),
			Define::Pipeline::PBR_Texture.c_str(),
			Define::Pipeline::Deferred_LightModel.c_str(),
			Define::Pipeline::Deferred_Volumn.c_str(),
			Define::Pipeline::Color.c_str(),
			Define::Pipeline::Dissolve.c_str(),
			Define::Pipeline::Water.c_str()};

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
		auto imageSize = 0;
		auto &info = material->info;
		if (info->pipelineName == Define::Pipeline::Skybox)
		{
			imageSize = 6;
		}
		else if (info->pipelineName == Define::Pipeline::LightModel ||
				 info->pipelineName == Define::Pipeline::Deferred_LightModel)
		{
			imageSize = 2;
		}
		else if (info->pipelineName == Define::Pipeline::PBR_Texture)
		{
			imageSize = 5;
		}
		else if (info->pipelineName == Define::Pipeline::Dissolve)
		{
			imageSize = 3;
		}
		if (imageSize > 0)
		{
			imageNameIndexs.resize(imageSize);
			for (auto i = 0; i < imageSize; i++)
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
		WindowLogic::GetDirectoryFiles("resource/texture", imageNames);

		imageNameCStrs.clear();
		for (auto &imageName : imageNames)
		{
			imageNameCStrs.push_back(imageName.c_str());
		}

		FindImageNameIndexsByPipelineName(material);
	}

	static bool DrawImageByIndex(std::shared_ptr<Render::Material> material,
								 const char *showName, int index)
	{
		if (ImGui::Combo(showName, &imageNameIndexs[index],
						 imageNameCStrs.data(), static_cast<int>(imageNameCStrs.size())))
		{
			auto &info = material->info;
			info->imageNames[index] = imageNames[imageNameIndexs[index]];
			info->hasChanged = true;
			return true;
		}
		return false;
	}

	template <>
	void ComponentWrap<Render::Material>::Draw(Context *context,
											   std::shared_ptr<void> component, bool isInit)
	{
		auto material = std::static_pointer_cast<Render::Material>(component);

		if (isInit)
		{
			DrawInit(context, material);
			return;
		}

		ImGui::Text("InstanceId: %d", material->data->id);

		auto &info = material->info;
		if (ImGui::Combo("##pipelineNames",
						 &pipelineNameIndex, pipelineNames, IM_ARRAYSIZE(pipelineNames)))
		{
			info->pipelineName = pipelineNames[pipelineNameIndex];
			info->hasChanged = true;

			if (info->pipelineName == Define::Pipeline::Skybox)
			{
				info->imageNames.resize(6, Define::Res::Img_White);
				info->params = {glm::vec4(0.0f)};
			}
			else if (info->pipelineName == Define::Pipeline::Color)
			{
				info->imageNames = {};
				info->params = {glm::vec4(1.0f)};
			}
			else if (info->pipelineName == Define::Pipeline::LightModel ||
					 info->pipelineName == Define::Pipeline::Deferred_LightModel)
			{
				info->imageNames = {Define::Res::Img_White, Define::Res::Img_Bump};
				info->params = {glm::vec4(1.0f, 50.0f, 1.0f, 1.0f)};
			}
			else if (info->pipelineName == Define::Pipeline::PBR_Simple)
			{
				info->imageNames = {};
				info->params = {glm::vec4(1.0f), glm::vec4(1.0f)};
			}
			else if (info->pipelineName == Define::Pipeline::PBR_Texture)
			{
				info->imageNames = {Define::Res::Img_White, Define::Res::Img_Bump,
									Define::Res::Img_White, Define::Res::Img_White,
									Define::Res::Img_BRDF_LUT};
				info->params = {glm::vec4(1.0f)};
			}
			if (info->pipelineName == Define::Pipeline::Dissolve)
			{
				info->imageNames.resize(3, Define::Res::Img_White);
				info->params = {glm::vec4(1.0f)};
			}
			if (info->pipelineName == Define::Pipeline::Dissolve)
			{
				info->imageNames = {};
				info->params = {glm::vec4(1.0f), glm::vec4(1.0f)};
			}
			else if (info->pipelineName == Define::Pipeline::Deferred_Volumn)
			{
				info->imageNames = {};
				info->params = {glm::vec4(1.0f)};
			}

			FindImageNameIndexsByPipelineName(material);
		}

		ImGui::Spacing();
		if (info->pipelineName == Define::Pipeline::Skybox)
		{
			auto &params0 = info->params[0];
			ImGui::SliderFloat("LodLevel", &params0.x, 0, 10);

			if (DrawImageByIndex(material, "+x", 0))
			{
				auto &imageName = info->imageNames[0];
				auto imageDir = imageName.substr(0, imageName.find_last_of("/") + 1);
				info->imageNames[0] = imageDir + "px.png";
				info->imageNames[1] = imageDir + "nx.png";
				info->imageNames[2] = imageDir + "py.png";
				info->imageNames[3] = imageDir + "ny.png";
				info->imageNames[4] = imageDir + "pz.png";
				info->imageNames[5] = imageDir + "nz.png";
			}
			// DrawImageByIndex(material, "-x", 1);
			// DrawImageByIndex(material, "+y", 2);
			// DrawImageByIndex(material, "-y", 3);
			// DrawImageByIndex(material, "+z", 4);
			// DrawImageByIndex(material, "-z", 5);
		}
		else if (info->pipelineName == Define::Pipeline::Color)
		{
			auto &params0 = info->params[0];
			ImGui::ColorEdit4("Color", &params0.x);
		}
		else if (info->pipelineName == Define::Pipeline::LightModel ||
				 info->pipelineName == Define::Pipeline::Deferred_LightModel)
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
		else if (info->pipelineName == Define::Pipeline::PBR_Simple)
		{
			ImGui::PushItemWidth(150);
			auto &params0 = info->params[0];
			ImGui::SliderFloat("Roughness", &params0.x, 0.05f, 1.0f);
			ImGui::SliderFloat("Metallic", &params0.y, 0.1f, 1.0f);
			ImGui::PopItemWidth();

			auto &params1 = info->params[1];
			ImGui::ColorEdit4("BaseColor", &params1.x);
		}
		else if (info->pipelineName == Define::Pipeline::PBR_Texture)
		{
			DrawImageByIndex(material, "Albedo", 0);
			DrawImageByIndex(material, "NomralMap", 1);
			DrawImageByIndex(material, "Roughness", 2);
			DrawImageByIndex(material, "Metallic", 3);
			DrawImageByIndex(material, "BRDF LUT", 4);

			ImGui::PushItemWidth(150);
			auto &params0 = info->params[0];

			ImGui::PushID("Use Reflection");
			ImGui::SliderFloat("Use Reflection", &params0.x, 0, 1);
			ImGui::PopID();

			ImGui::PopItemWidth();
		}
		else if (info->pipelineName == Define::Pipeline::Dissolve)
		{
			DrawImageByIndex(material, "Albedo", 0);
			DrawImageByIndex(material, "Dissolve", 1);
			DrawImageByIndex(material, "Ramp", 2);

			ImGui::PushItemWidth(150);
			auto &params0 = info->params[0];
			ImGui::SliderFloat("Clip", &params0.x, 0.0f, 1.0f);
			ImGui::SliderFloat("Alpha", &params0.y, 0.0f, 1.0f);
			ImGui::PopItemWidth();
		}
		else if (info->pipelineName == Define::Pipeline::Water)
		{
			ImGui::PushItemWidth(150);
			auto &params0 = info->params[0];
			ImGui::InputFloat("Angle", &params0.x);
			ImGui::SliderFloat("Steepness01", &params0.y, 0.0f, 1.0f);
			ImGui::InputFloat("Length", &params0.z);
			ImGui::InputFloat("Speed", &params0.w);
			auto &params1 = info->params[1];
			ImGui::InputFloat("DiffuseIntensity", &params1.x);
			ImGui::InputFloat("SpecularShininess", &params1.y);
			ImGui::InputFloat("SpecularIntensity", &params1.z);
			ImGui::PopItemWidth();
		}

		ImGui::Checkbox("CastShadow", &info->castShadow);
		ImGui::InputInt("RenderQueue", &info->renderQueue);
	}
}