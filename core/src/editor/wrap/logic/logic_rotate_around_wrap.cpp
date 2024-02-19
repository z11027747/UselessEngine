
#include <imgui/imgui.h>
#include <memory>
#include "editor/wrap/component_wrap.h"
#include "logic/transform/transform_comp.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    template <>
    void ComponentWrap<Logic::RotateAround>::Draw(Context *context,
                                                  std::shared_ptr<void> component, bool isInit)
    {
        auto rotateAround = std::static_pointer_cast<Logic::RotateAround>(component);

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