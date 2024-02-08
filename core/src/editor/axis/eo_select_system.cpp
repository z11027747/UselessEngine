
#include <GLFW/glfw3.h>
#include "logic/hit/hit_ray_logic.h"
#include "editor/axis/axis_system.h"
#include "editor/window/window_logic.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    static bool lastMousePress = false;

    void EOSelectSystem::Update(Context *context)
    {
        if (!WindowLogic::IsInViewport(context))
            return;

        auto &window = context->window;
        auto deltaTime = context->deltaTime;

        double currX, currY;
        glfwGetCursorPos(window, &currX, &currY);

        auto isMousePress = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);

        if (!lastMousePress && isMousePress)
        {
            auto ndcX = (float)currX;
            auto ndcY = (float)currY;
            WindowLogic::ToViewportNdcXY(context, ndcX, ndcY);

            auto tempHitEO = Logic::HitRayCheckLogic::TestFromNdc(context, ndcX, ndcY);
            if (tempHitEO != nullptr)
            {
                WindowLogic::SetSelectEO(context, tempHitEO);
            }
        }

        lastMousePress = isMousePress;
    }
}