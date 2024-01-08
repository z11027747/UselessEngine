
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include "editor/wrap/component_wrap.h"
#include "editor/global.h"
#include "context.h"

namespace Editor
{
	static char pipelineName[32] = "";
	static char objName[32] = "";
	static char textureName[32] = "";

	void ComponentWrap<Render::Unit>::Draw(Context *context,
										   std::shared_ptr<Render::Unit> unit, bool isFirst)
	{
		if (isFirst)
		{
			memcpy(pipelineName, unit->pipelineName.data(), unit->pipelineName.size());
			memcpy(objName, unit->objName.data(), unit->objName.size());
			memcpy(textureName, unit->textureName.data(), unit->textureName.size());
		}

		if (ImGui::InputText("PipelineName", pipelineName, IM_ARRAYSIZE(pipelineName)))
		{
		}

		if (ImGui::InputText("ObjName", objName, IM_ARRAYSIZE(objName)))
		{
		}

		if (ImGui::InputText("TextureName", textureName, IM_ARRAYSIZE(textureName)))
		{
		}

		ImGui::Checkbox("CastShadow", &unit->castShadow);
	}
}