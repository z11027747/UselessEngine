
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
    void EOSelectSystem::Update(Context *context)
    {
        if (!Window::IsInViewport(context))
            return;

        auto &window = context->window;
        auto deltaTime = context->deltaTime;

        double currX, currY;
        glfwGetCursorPos(window, &currX, &currY);

        auto isMousePress = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
        if (isMousePress)
        {
            auto ndcX = (float)currX;
            auto ndcY = (float)currY;
            Window::ToViewportNdcXY(context, ndcX, ndcY);

            auto tempHitEO = Logic::HitRayCheckLogic::TestFromNdc(context, ndcX, ndcY);
            if (tempHitEO != nullptr)
            {
                Window::SetSelectEO(context, tempHitEO);
            }
        }
    }
}