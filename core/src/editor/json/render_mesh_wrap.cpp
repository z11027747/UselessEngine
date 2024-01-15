
#include "editor/json/component_json.h"
#include "render/mesh/mesh_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<Render::Mesh> ComponentJson<Render::Mesh>::From(Context *context, const json11::Json &j)
    {
        auto mesh = std::make_shared<Render::Mesh>();
        mesh->objName = j["objName"].string_value();

        return mesh;
    }

    template <>
    json11::Json ComponentJson<Render::Mesh>::To(Context *context,
                                                 std::shared_ptr<Render::Mesh> mesh)
    {
        auto &objName = mesh->objName;

        auto jObj = json11::Json::object{
            {"type", "material"},
            {"objName", objName}};

        return jObj;
    }
}