
#include <GLFW/glfw3.h>
#include "logic/transform/transform_logic.h"
#include "logic/hit/hit_ray_logic.h"
#include "logic/camera/camera_logic.h"
#include "editor/system.h"
#include "editor/window.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
    static std::shared_ptr<EngineObject> hitEO = nullptr;
    static float lastX, lastY = 0.0f;
    static float moveSpeed = 10.0f;

    void AxisSelectSystem::Update(Context *context)
    {
        auto &window = context->window;
        auto deltaTime = context->deltaTime;

        double currX, currY;
        glfwGetCursorPos(window, &currX, &currY);

        if (Window::IsInViewport(context) && Window::selectEO != nullptr)
        {
            auto isMousePress = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
            auto isMouseRelease = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE);

            auto ndcX = (float)currX;
            auto ndcY = (float)currY;
            Window::ToViewportNdcXY(context, ndcX, ndcY);

            auto ray = Logic::HitRayCheckLogic::CalcaRayFromNdc(context, ndcX, ndcY);
            std::shared_ptr<EngineObject> tempHitEO = nullptr;

            auto &axisHitEOs = context->logicAxisHitEOs;
            for (const auto &axisHitEO : axisHitEOs)
            {
                auto result = Logic::HitRayCheckLogic::Test(context,
                                                            ray, axisHitEO);
                if (result)
                {
                    tempHitEO = axisHitEO;
                    break;
                }
            }

            if (hitEO == nullptr && tempHitEO != hitEO)
            {
                hitEO = tempHitEO;
                Logic::TransformLogic::SetScale(tempHitEO, 1.2f);
            }
            if (tempHitEO == nullptr && tempHitEO != hitEO && isMouseRelease)
            {
                Logic::TransformLogic::SetScale(hitEO, 1.0f);
                hitEO = nullptr;
            }

            if (hitEO != nullptr && isMousePress)
            {
                auto mainCamera = context->logicMainCameraEO->GetComponent<Logic::Camera>();
                auto &view = mainCamera->view;

                auto selectTransform = Window::selectEO->GetComponent<Logic::Transform>();

                auto offsetX = (float)currX - lastX;
                auto offsetY = lastY - (float)currY;
                auto offset = glm::vec3(offsetX, offsetY, 0.0f);

                if (hitEO->name == Name_AxisX)
                {
                    auto viewXDir = view * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
                    auto dotX = glm::dot(offset, glm::vec3(viewXDir));
                    selectTransform->localPosition.x += dotX * moveSpeed * deltaTime;
                }
                else if (hitEO->name == Name_AxisY)
                {
                    auto viewYDir = view * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
                    auto dotY = glm::dot(offset, glm::vec3(viewYDir));
                    selectTransform->localPosition.y += dotY * moveSpeed * deltaTime;
                }
                else if (hitEO->name == Name_AxisZ)
                {
                    auto viewZDir = view * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
                    auto dotZ = glm::dot(offset, glm::vec3(viewZDir));
                    selectTransform->localPosition.z += dotZ * moveSpeed * deltaTime;
                }
            }
        }

        lastX = (float)currX;
        lastY = (float)currY;
    }
}