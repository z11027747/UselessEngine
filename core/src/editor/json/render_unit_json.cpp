
#include "editor/json/component_json.h"
#include "render/unit/unit_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<Render::Unit> ComponentJson<Render::Unit>::From(Context *context, const json11::Json &j)
    {
        auto unit = std::make_shared<Render::Unit>();

        return unit;
    }

    template <>
    json11::Json ComponentJson<Render::Unit>::To(Context *context,
                                                 std::shared_ptr<Render::Unit> unit)
    {
        auto jObj = json11::Json::object{
            {"type", Type_Render_Unit}};

        return jObj;
    }
}