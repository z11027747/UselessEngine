
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include "editor/wrap/component_wrap.h"
#include "editor/system.h"
#include "context.h"

namespace Editor
{
	static char objName[32] = "";

	void ComponentWrap<Render::Mesh>::Draw(Context *context,
										   std::shared_ptr<Render::Mesh> mesh, bool isFirst)
	{
		if (isFirst)
		{
			memcpy(objName, mesh->objName.data(), mesh->objName.size());
		}

		if (ImGui::InputText("ObjName", objName, IM_ARRAYSIZE(objName)))
		{
		}
	}
}