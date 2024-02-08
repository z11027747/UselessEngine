#pragma once

#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/post_process/post_process_comp.h"
#include "logic/transform/transform_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/rotate/rotate_comp.h"
#include "editor/wrap/wrap_system.hpp"
#include "editor/wrap/component_wrap.h"
#include "context.hpp"

namespace Editor
{
    void WrapRegistSystem::Create(Context *context)
    {
        REGIST_WRAP(Logic::Transform);
        REGIST_WRAP(Logic::Camera);
        REGIST_WRAP(Logic::RotateAround);
        REGIST_WRAP(Render::DirectionLight);
        REGIST_WRAP(Render::PointLight);
        REGIST_WRAP(Render::Mesh);
        REGIST_WRAP(Render::Material);
        REGIST_WRAP(Render::PostProcess);
    }
}