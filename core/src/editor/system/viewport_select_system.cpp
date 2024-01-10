
#include <GLFW/glfw3.h>
#include "logic/hit/hit_ray_logic.h"
#include "editor/system.h"
#include "editor/window.h"
#include "context.h"

namespace Editor
{
    void ViewportSelectSystem::Update(Context *context)
    {
        if (!Window::IsInViewport(context))
            return;

        auto window = context->window;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_PRESS)
            return;

        double currX, currY;
        glfwGetCursorPos(window, &currX, &currY);

        auto ndcX = (float)currX;
        auto ndcY = (float)currY;
        Window::ToViewportNdcXY(context, ndcX, ndcY);

        Logic::HitRayCheckLogic::FromNdc(context, ndcX, ndcY);
    }
}