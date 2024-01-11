
#include <GLFW/glfw3.h>
#include "logic/camera/camera_logic.h"
#include "logic/camera/camera_system.h"
#include "editor/window.h"
#include "editor/system.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
    static float rotateSpeed = 50.0f;
    static float rotateLastX, rotateLastY;

    void CameraRotateSystem::Update(Context *context)
    {
        if (!Editor::Window::IsInViewport(context))
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