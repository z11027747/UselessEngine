
#include "logic/camera/camera_logic.h"
#include "logic/camera/camera_system.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void CameraCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(Camera);
        REGIST_ADD_REMOVE(Camera, CameraLogic);
        REGIST_JSON(Camera, CameraJson);
    }
}