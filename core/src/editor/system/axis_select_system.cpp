
#include <GLFW/glfw3.h>
#include "logic/transform/transform_logic.h"
#include "logic/hit/hit_ray_logic.h"
#include "logic/camera/camera_logic.h"
#include "editor/system.h"
#include "editor/window.h"
#include "common/log_system.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
    static std::shared_ptr<EngineObject> lastAxisEO = nullptr;
    static float lastX, lastY = 0.0f;
    // static float moveSpeed = 10.0f;

    bool AxisSelectSystem::Update(Context *context)
    {
        auto &window = context->window;
        auto deltaTime = context->deltaTime;

        double currX, currY;
        glfwGetCursorPos(window, &currX, &currY);

        auto &selectEO = Window::selectEO;

        if (Window::IsInViewport(context) && selectEO != nullptr)
        {
            auto isMousePress = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
            auto isMouseRelease = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE);

            auto ndcX = (float)currX;
            auto ndcY = (float)currY;
            Window::ToViewportNdcXY(context, ndcX, ndcY);

            auto ray = Logic::HitRayCheckLogic::CalcaRayFromNdc(context, ndcX, ndcY);
            std::shared_ptr<EngineObject> currAxisHitEO = nullptr;

            auto &axisEOs = context->editorAxisEOs;
            for (const auto &axisEO : axisEOs)
            {
                auto result = Logic::HitRayCheckLogic::Test(context,
                                                            ray, axisEO);
                if (result)
                {
                    currAxisHitEO = axisEO;
                    break;
                }
            }

            if (lastAxisEO == nullptr && currAxisHitEO != nullptr)
            {
                lastAxisEO = currAxisHitEO;
                Logic::TransformLogic::SetScale(currAxisHitEO, 1.2f);
            }
            if (lastAxisEO != nullptr && currAxisHitEO == nullptr && isMouseRelease)
            {
                Logic::TransformLogic::SetScale(lastAxisEO, 1.0f);
                lastAxisEO = nullptr;
            }

            if (lastAxisEO != nullptr && isMousePress)
            {
                auto mainCameraEO = context->logicMainCameraEO;
                auto mainCameraTransform = mainCameraEO->GetComponent<Logic::Transform>();

                auto selectTransform = selectEO->GetComponent<Logic::Transform>();

                auto distance = glm::distance(mainCameraTransform->worldPosition, selectTransform->worldPosition);
                auto moveSpeed = distance * 0.6f;
                // Common::LogSystem::Debug(moveSpeed);

                auto offsetX = (float)currX - lastX;
                auto offsetY = lastY - (float)currY;
                auto offset = glm::vec3(offsetX, offsetY, 0.0f);

                if (lastAxisEO->name == Name_AxisX)
                {
                    auto dotX = glm::dot(offset, mainCameraTransform->right);
                    selectTransform->localPosition.x += dotX * moveSpeed * deltaTime;
                }
                else if (lastAxisEO->name == Name_AxisY)
                {
                    auto dotY = glm::dot(offset, mainCameraTransform->up);
                    selectTransform->localPosition.y += dotY * moveSpeed * deltaTime;
                }
                else if (lastAxisEO->name == Name_AxisZ)
                {
                    auto dotZ = glm::dot(offset, -mainCameraTransform->forward);
                    selectTransform->localPosition.z += dotZ * moveSpeed * deltaTime;
                }
            }
        }

        lastX = (float)currX;
        lastY = (float)currY;

        return (lastAxisEO != nullptr);
    }
}