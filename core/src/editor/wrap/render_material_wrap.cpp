
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/component_wrap.h"
#include "context.h"

namespace Editor
{
	static char pipelineName[32] = "";
	static char image0Name[32] = "";

	void ComponentWrap<Render::Material>::Draw(Context *context,
											   std::shared_ptr<Render::Material> material, bool isFirst)
	{
		if (isFirst)
		{
			memcpy(pipelineName, material->pipelineName.data(), material->pipelineName.size());
			memcpy(image0Name, material->image0Name.data(), material->image0Name.size());
			return;
		}

		if (ImGui::InputText("PipelineName", pipelineName, IM_ARRAYSIZE(pipelineName)))
		{
		}

		if (ImGui::InputText("Image0Name", image0Name, IM_ARRAYSIZE(image0Name)))
		{
		}

		ImGui::Checkbox("CastShadow", &material->castShadow);
	}
}