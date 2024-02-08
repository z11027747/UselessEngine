
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "logic/transform/transform_logic.h"
#include "editor/camera/camera_system.hpp"
#include "editor/window/window_logic.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    static float rotateSpeed = 50.0f;
    static float rotateLastX, rotateLastY;

    void CameraRotateSystem::Update(Context *context)
    {
        if (!WindowLogic::IsInViewport(context))
            return;

        auto &mainCamera = context->logicMainCameraEO;
        auto mainCameraTransform = mainCamera->GetComponent<Logic::Transform>();

        auto &window = context->window;
        auto deltaTime = context->deltaTime;

        double currX, currY;
        glfwGetCursorPos(window, &currX, &currY);

        auto isMousePress = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS);
        if (isMousePress)
        {
            auto offsetX = (float)currX - rotateLastX;
            auto offsetY = (float)currY - rotateLastY;

            mainCameraTransform->localEulerAngles.y += offsetX * rotateSpeed * deltaTime;
            mainCameraTransform->localEulerAngles.x += offsetY * rotateSpeed * deltaTime;
        }

        rotateLastX = (float)currX;
        rotateLastY = (float)currY;
    }
}