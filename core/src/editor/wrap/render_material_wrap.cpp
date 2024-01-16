
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
										  Render::Pipeline_Bling_Phone.c_str(),
										  Render::Pipeline_Color.c_str()};

	static int image0NameIndex = -1;
	static std::vector<std::string> image0Names = {};
	static std::vector<const char *> image0NameCStrs = {};

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

			image0NameIndex = -1;
			image0Names.clear();
			Window::GetDirectoryFiles("resource/texture", image0Names);

			auto &image0Name = material->image0Names[0];
			auto it = std::find(image0Names.begin(), image0Names.end(), image0Name);
			if (it != image0Names.end())
			{
				auto index = std::distance(image0Names.begin(), it);
				image0NameIndex = static_cast<int>(index);
			}

			image0NameCStrs.clear();
			for (auto &image0Name : image0Names)
			{
				image0NameCStrs.push_back(image0Name.c_str());
			}

			return;
		}

		if (ImGui::Combo("##pipelineNames",
						 &pipelineNameIndex, pipelineNames, IM_ARRAYSIZE(pipelineNames)))
		{
		}

		if (ImGui::Combo("Image0Name", &image0NameIndex,
						 image0NameCStrs.data(), static_cast<int>(image0NameCStrs.size())))
		{
			material->image0Names[0] = image0Names[image0NameIndex];
			material->hasChanged = true;
		}

		ImGui::Checkbox("CastShadow", &material->castShadow);
	}
}