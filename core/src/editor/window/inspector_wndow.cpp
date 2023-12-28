
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include <string>
#include "editor/wrap/engine_object_wrap.h"
#include "editor/window/inspector_window.h"
#include "editor/global.h"
#include "logic/transform/transform_comp.h"
#include "editor/wrap/logic_transform_wrap.h"
#include "logic/camera/camera_comp.h"
#include "editor/wrap/logic_camera_wrap.h"
#include "context.h"
#include "engine_object.h"

namespace Editor {

	std::shared_ptr<EngineObject> InspectorWindow::selectEO = nullptr;

	static char addCompName[16] = "";

	void InspectorWindow::Draw(Context* context) {
		if (ImGui::Begin("Inspector", NULL)) {

			if (selectEO == nullptr) {
				ImGui::Text("Please Select an EngineObject");
			}
			else {
				ImGui::Text("Select EngineObject: %s", selectEO->name.c_str());

				ImGui::SeparatorText("ComponentList Begin"); {

					auto index = 0;
					auto& componentMap = selectEO->componentMap;
					for (const auto& kv : componentMap) {
						auto typeId = kv.first;
						auto& component = kv.second;

						ImGui::PushID(index++);
						if (ImGui::TreeNode("Comp: &s", typeId.name())) {

							if (typeId == typeid(Logic::Transform)) {
								auto transform = std::static_pointer_cast<Logic::Transform>(component);
								LogicTransformWrap::Draw(context,
									transform);
							}
							else if (typeId == typeid(Logic::Camera)) {
								auto camera = std::static_pointer_cast<Logic::Camera>(component);
								LogicCameraWrap::Draw(context,
									camera);
							}

							ImGui::TreePop();
						}
						ImGui::PopID();
					}
				}
				ImGui::SeparatorText("End");

				ImGui::SetNextItemWidth(150.0f);
				ImGui::InputText("##addCompName", addCompName, IM_ARRAYSIZE(addCompName));
				ImGui::SameLine();
				if (ImGui::Button("Add Component")) {
					std::cout << "Add Component Click!" << std::endl;

					memset(addCompName, 0, sizeof(addCompName));
				}
			}

			ImGui::End();
		}
	}

}