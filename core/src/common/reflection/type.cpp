
#include <unordered_map>
#include <functional>
#include "render/light/light_comp.h"
#include "render/light/light_logic.h"
#include "render/material/material_comp.h"
#include "render/material/material_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "logic/hit/hit_comp.h"
#include "logic/move/move_comp.h"
#include "logic/move/move_logic.h"
#include "logic/rotate/rotate_comp.h"
#include "logic/rotate/rotate_logic.h"
#include "logic/transform/transform_comp.h"
#include "editor/axis/axis_comp.h"
#include "editor/axis/axis_logic.h"
#include "common/reflection/type.h"
#include "engine_object.h"
#include "context.h"

namespace Common
{
#define TYPE_TO_CONSTRUCT(class)                                 \
    {                                                            \
        class ::type, []() { return std::make_shared<class>(); } \
    }
    static std::unordered_map<std::string, std::function<std::shared_ptr<void>()>>
        map{
            // logic
            TYPE_TO_CONSTRUCT(Logic::Camera),
            TYPE_TO_CONSTRUCT(Logic::HitRay),
            TYPE_TO_CONSTRUCT(Logic::MoveFowrard),
            TYPE_TO_CONSTRUCT(Logic::MoveFollow),
            TYPE_TO_CONSTRUCT(Logic::RotateAround),
            TYPE_TO_CONSTRUCT(Logic::Transform),
            // render
            TYPE_TO_CONSTRUCT(Render::DirectionLight),
            TYPE_TO_CONSTRUCT(Render::PointLight),
            TYPE_TO_CONSTRUCT(Render::SpotLight),
            TYPE_TO_CONSTRUCT(Render::Material),
            TYPE_TO_CONSTRUCT(Render::Mesh),
        };

    std::shared_ptr<void> Type::Create(const std::string &type)
    {
        return map[type]();
    }

#define TYPE_TO_ADD(class, logicClass)                                                                 \
    {                                                                                                  \
        class ::type,                                                                                  \
            [](Context *context, std::shared_ptr<EngineObject> eo) { logicClass::OnAdd(context, eo); } \
    }

    static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<EngineObject>)>>
        addMap{
            // editor
            TYPE_TO_ADD(Editor::Axis, Editor::AxisLogic),
            // logic
            TYPE_TO_ADD(Logic::Camera, Logic::CameraLogic),
            TYPE_TO_ADD(Logic::MoveFowrard, Logic::MoveLogic),
            TYPE_TO_ADD(Logic::MoveFollow, Logic::MoveLogic),
            TYPE_TO_ADD(Logic::RotateAround, Logic::RotateLogic),
            // render
            TYPE_TO_ADD(Render::DirectionLight, Render::LightLogic),
            TYPE_TO_ADD(Render::PointLight, Render::LightLogic),
            TYPE_TO_ADD(Render::Material, Render::MaterialLogic),
            TYPE_TO_ADD(Render::Mesh, Render::MeshLogic),
        };

    void Type::OnAdd(const std::string &type,
                     Context *context, std::shared_ptr<EngineObject> eo)
    {
        auto it = addMap.find(type);
        if (it == addMap.end())
            return;

        return addMap[type](context, eo);
    }

#define TYPE_TO_REMOVE(class, logicClass)                                                                 \
    {                                                                                                     \
        class ::type,                                                                                     \
            [](Context *context, std::shared_ptr<EngineObject> eo) { logicClass::OnRemove(context, eo); } \
    }

    static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<EngineObject>)>>
        removeMap{
            // editor
            TYPE_TO_REMOVE(Editor::Axis, Editor::AxisLogic),
            // logic
            TYPE_TO_REMOVE(Logic::Camera, Logic::CameraLogic),
            TYPE_TO_REMOVE(Logic::MoveFowrard, Logic::MoveLogic),
            TYPE_TO_REMOVE(Logic::MoveFollow, Logic::MoveLogic),
            TYPE_TO_REMOVE(Logic::RotateAround, Logic::RotateLogic),
            // render
            TYPE_TO_REMOVE(Render::DirectionLight, Render::LightLogic),
            TYPE_TO_REMOVE(Render::PointLight, Render::LightLogic),
            TYPE_TO_REMOVE(Render::Material, Render::MaterialLogic),
            TYPE_TO_REMOVE(Render::Mesh, Render::MeshLogic),
        };

    void Type::OnRemove(const std::string &type,
                        Context *context, std::shared_ptr<EngineObject> eo)
    {
        auto it = removeMap.find(type);
        if (it == removeMap.end())
            return;

        return removeMap[type](context, eo);
    }
}