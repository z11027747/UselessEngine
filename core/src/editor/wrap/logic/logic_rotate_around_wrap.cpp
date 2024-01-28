
#include <imgui/imgui.h>
#include <memory>
#include "editor/wrap/component_wrap.h"
#include "logic/rotate/rotate_comp.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
    template <>
    void ComponentWrap<Logic::RotateAround>::Draw(Context *context,
                                                  std::shared_ptr<Logic::RotateAround> rotateAround, bool isInit)
    {
        if (isInit)
        {
            return;
        }

        auto &targetEO = rotateAround->targetEO;
        if (targetEO != nullptr)
        {
            auto &targetName = targetEO->name;
            ImGui::Text("TargetEO: %s", targetName.data(),
                        static_cast<int>(targetName.size()));
        }

        ImGui::InputFloat3("Offset", &rotateAround->offset.x);
        ImGui::InputFloat("Angle Speed", &rotateAround->angleSpeed);
    }
}