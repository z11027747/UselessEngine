
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "logic/transform/transform_logic.h"
#include "editor/camera/camera_system.hpp"
#include "editor/window/window_logic.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    static float moveSpeed = 5.0f;

    void CameraMoveSystem::Update(Context *context)
    {
        if (!WindowLogic::IsInViewport(context))
            return;

        auto &mainCamera = context->logicMainCameraEO;
        auto mainCameraTransform = mainCamera->GetComponent<Logic::Transform>();

        auto &window = context->window;
        auto deltaTime = context->deltaTime;

        auto moveDir = glm::vec3(0.0f);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            moveDir += mainCameraTransform->forward;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            moveDir -= mainCameraTransform->forward;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            moveDir -= mainCameraTransform->right;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            moveDir += mainCameraTransform->right;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            moveDir -= mainCameraTransform->up;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            moveDir += mainCameraTransform->up;

        if (moveDir != glm::vec3(0.0f))
        {
            mainCameraTransform->localPosition += glm::normalize(moveDir) * moveSpeed * deltaTime;
        }
    }
}