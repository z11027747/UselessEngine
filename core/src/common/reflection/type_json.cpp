
#include <unordered_map>
#include <functional>
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/post_process/post_process_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/hit/hit_comp.h"
#include "logic/move/move_comp.h"
#include "logic/rotate/rotate_comp.h"
#include "logic/transform/transform_comp.h"
#include "common/reflection/type_json.h"
#include "editor/axis/axis_comp.h"

namespace Common
{
#define TYPE_JSON_FROM(class)                                                              \
    {                                                                                      \
        class ::type, [&](const json11::Json &j) { return ComponentJson<class>::From(j); } \
    }
    static std::unordered_map<std::string, std::function<std::shared_ptr<void>(const json11::Json &)>>
        fromMap{
            // editor
            TYPE_JSON_FROM(Editor::Axis),
            // logic
            TYPE_JSON_FROM(Logic::Camera),
            TYPE_JSON_FROM(Logic::Transform),
            // render
            TYPE_JSON_FROM(Render::DirectionLight),
            TYPE_JSON_FROM(Render::PointLight),
            // TYPE_JSON_FROM(Render::SpotLight),
            TYPE_JSON_FROM(Render::Material),
            TYPE_JSON_FROM(Render::Mesh),
            TYPE_JSON_FROM(Render::PostProcess),
        };

    std::shared_ptr<void> TypeJson::From(const std::string &type, const json11::Json &j)
    {
        auto it = fromMap.find(type);
        if (it == fromMap.end())
            return nullptr;

        return fromMap[type](j);
    }

#define TYPE_JSON_TO(class)                                                                                \
    {                                                                                                      \
        class ::type, [&](std::shared_ptr<void> component) { return ComponentJson<class>::To(component); } \
    }
    static std::unordered_map<std::string, std::function<json11::Json(std::shared_ptr<void>)>>
        toMap{
            // editor
            TYPE_JSON_TO(Editor::Axis),
            // logic
            TYPE_JSON_TO(Logic::Camera),
            TYPE_JSON_TO(Logic::Transform),
            // render
            TYPE_JSON_TO(Render::DirectionLight),
            TYPE_JSON_TO(Render::PointLight),
            // TYPE_JSON_TO(Render::SpotLight),
            TYPE_JSON_TO(Render::Material),
            TYPE_JSON_TO(Render::Mesh),
            TYPE_JSON_TO(Render::PostProcess),
        };

    json11::Json TypeJson::To(const std::string &type, std::shared_ptr<void> component)
    {
        auto it = toMap.find(type);
        if (it == toMap.end())
            return {};

        return toMap[type](component);
    }
}