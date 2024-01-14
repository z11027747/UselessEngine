
#include "editor/json/component_json.h"
#include "render/material/material_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    json11::Json ComponentJson<Render::Material>::To(Context *context,
                                                     std::shared_ptr<Render::Material> material)

    {
        auto &pipelineName = material->pipelineName;
        auto &image0Name = material->image0Name;
        auto castShadow = material->castShadow;

        auto jObj = json11::Json::object{{"pipelineName", pipelineName},
                                         {"image0Name", image0Name},
                                         {"castShadow", castShadow}};

        return json11::Json::object{{"material", jObj}};
    }
}