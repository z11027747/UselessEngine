
#include "editor/json/component_json.h"
#include "render/mesh/mesh_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    json11::Json ComponentJson<Render::Mesh>::To(Context *context,
                                                 std::shared_ptr<Render::Mesh> mesh)

    {
        auto &objName = mesh->objName;

        auto jObj = json11::Json::object{{"objName", objName}};

        return json11::Json::object{{"material", jObj}};
    }
}