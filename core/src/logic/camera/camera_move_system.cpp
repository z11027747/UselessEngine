
#include <GLFW/glfw3.h>
#include "logic/transform/transform_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "logic/camera/camera_system.h"
#include "context.h"
#include "engine_object.h"

namespace Logic
{
    void CameraMoveSystem::Update(Context *context)
    {
        auto &cameraEO = context->mainCameraEO;
        auto cameraTransform = cameraEO->GetComponent<Transform>();

        auto moveDir = glm::vec3(0.0f);
        auto moveSpeed = 5.0f;

        if (glfwGetKey(context->window, GLFW_KEY_W) == GLFW_PRESS)
            moveDir = cameraTransform->forward;

        if (glfwGetKey(context->window, GLFW_KEY_S) == GLFW_PRESS)
            moveDir = -cameraTransform->forward;

        if (glfwGetKey(context->window, GLFW_KEY_A) == GLFW_PRESS)
            moveDir = -cameraTransform->right;

        if (glfwGetKey(context->window, GLFW_KEY_D) == GLFW_PRESS)
            moveDir = cameraTransform->right;

        if (glfwGetKey(context->window, GLFW_KEY_Q) == GLFW_PRESS)
            moveDir = -cameraTransform->up;

        if (glfwGetKey(context->window, GLFW_KEY_E) == GLFW_PRESS)
            moveDir = cameraTransform->up;

        cameraTransform->position += moveDir * moveSpeed * context->deltaTime;
    }
}