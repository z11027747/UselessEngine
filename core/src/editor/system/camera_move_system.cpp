
#include <GLFW/glfw3.h>
#include "logic/camera/camera_logic.h"
#include "logic/camera/camera_system.h"
#include "editor/window.h"
#include "editor/system.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
    static float moveSpeed = 5.0f;

    static void UpdatePos(Context *context,
                          std::shared_ptr<Logic::Transform> transform)
    {
        auto &window = context->window;
        auto deltaTime = context->deltaTime;

        auto moveDir = glm::vec3(0.0f);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            moveDir += transform->forward;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            moveDir -= transform->forward;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            moveDir -= transform->right;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            moveDir += transform->right;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            moveDir -= transform->up;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            moveDir += transform->up;

        if (moveDir != glm::vec3(0.0f))
        {
            transform->localPosition += glm::normalize(moveDir) * moveSpeed * deltaTime;
        }
    }

    static bool rotateEnabled = false;
    static float rotateSpeed = 50.0f;
    static float rotateLastX, rotateLastY;

    static void UpdateRot(Context *context,
                          std::shared_ptr<Logic::Transform> transform)
    {
        auto &window = context->window;
        auto deltaTime = context->deltaTime;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
            rotateEnabled = true;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE)
            rotateEnabled = false;

        double currX, currY;
        glfwGetCursorPos(window, &currX, &currY);

        if (rotateEnabled)
        {
            auto offsetX = (float)currX - rotateLastX;
            auto offsetY = (float)currY - rotateLastY;

            transform->localEulerAngles.y += offsetX * rotateSpeed * deltaTime;
            transform->localEulerAngles.x += offsetY * rotateSpeed * deltaTime;
        }

        rotateLastX = (float)currX;
        rotateLastY = (float)currY;
    }

    void CameraMoveSystem::Update(Context *context)
    {
        if (!Editor::Window::IsInViewport(context))
            return;

        auto &mainCamera = context->logicMainCameraEO;
        auto mainCameraTransform = mainCamera->GetComponent<Logic::Transform>();

        UpdatePos(context, mainCameraTransform);
        UpdateRot(context, mainCameraTransform);
    }
}