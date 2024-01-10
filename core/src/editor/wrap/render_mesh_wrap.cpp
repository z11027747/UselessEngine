
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/component_wrap.h"
#include "context.h"

namespace Editor
{
	static char objName[32] = "";
	static float boundCenter[3] = {0.0f, 0.0f, 0.0f};
	static float boundRadius = 1.0f;

	void ComponentWrap<Render::Mesh>::Draw(Context *context,
										   std::shared_ptr<Render::Mesh> mesh, bool isFirst)
	{
		if (isFirst)
		{
			memcpy(objName, mesh->objName.data(), mesh->objName.size());
			boundCenter[0] = mesh->bound.center.x;
			boundCenter[1] = mesh->bound.center.y;
			boundCenter[2] = mesh->bound.center.z;
			boundRadius = mesh->bound.radius;
			return;
		}

		if (ImGui::InputText("ObjName", objName, IM_ARRAYSIZE(objName)))
		{
		}

		if (ImGui::InputFloat3("BoundCenter", boundCenter))
		{
			mesh->bound.center.x = boundCenter[0];
			mesh->bound.center.y = boundCenter[1];
			mesh->bound.center.z = boundCenter[2];
		}

		if (ImGui::InputFloat("BoundRadius", &boundRadius))
		{
			mesh->bound.radius = boundRadius;
		}
	}
}