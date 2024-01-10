
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

    void ViewportSelectSystem::Update(Context *context)
    {
        if (!Window::IsInViewport(context))
            return;

        auto &window = context->window;
        auto deltaTime = context->deltaTime;

        double currX, currY;
        glfwGetCursorPos(window, &currX, &currY);

        auto ndcX = (float)currX;
        auto ndcY = (float)currY;
        Window::ToViewportNdcXY(context, ndcX, ndcY);

        auto tempHitEO = Logic::HitRayCheckLogic::FromNdc(context, ndcX, ndcY);
        if (hitEO == nullptr && tempHitEO != hitEO)
        {
            hitEO = tempHitEO;
            Logic::TransformLogic::SetScale(hitEO, 1.2f);
        }
        if (hitEO != nullptr && tempHitEO == nullptr &&
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
        {
            Logic::TransformLogic::SetScale(hitEO, 1.0f);
            hitEO = nullptr;
        }

        if (hitEO != nullptr &&
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
        {
            auto selectTransform = Window::selectEO->GetComponent<Logic::Transform>();

            auto offsetX = (float)currX - lastX;
            auto offsetY = lastY - (float)currY;

            // TODO
            if (hitEO->name == "X")
                selectTransform->localPosition.x += offsetX * moveSpeed * deltaTime;
        }

        lastX = (float)currX;
        lastY = (float)currY;
    }
}