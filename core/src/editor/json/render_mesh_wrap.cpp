
#include "editor/json/component_json.h"
#include "render/mesh/mesh_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<Render::Mesh> ComponentJson<Render::Mesh>::From(Context *context, const json11::Json &j)
    {
        auto &defaultColorJArr = j["defaultColor"].array_items();
        auto localPositionR = (float)defaultColorJArr.at(0).number_value();
        auto localPositionG = (float)defaultColorJArr.at(1).number_value();
        auto localPositionB = (float)defaultColorJArr.at(2).number_value();

        auto mesh = std::make_shared<Render::Mesh>();
        mesh->objName = j["objName"].string_value();
        mesh->defaultColor = glm::vec3(localPositionR, localPositionG, localPositionB);
        mesh->scaler = j["scaler"].number_value();
        mesh->checkHit = j["checkHit"].bool_value();

        return mesh;
    }

    template <>
    json11::Json ComponentJson<Render::Mesh>::To(Context *context,
                                                 std::shared_ptr<Render::Mesh> mesh)
    {
        auto &defaultColor = mesh->defaultColor;

        auto defaultColorJArr = json11::Json::array{defaultColor.r, defaultColor.g, defaultColor.b};

        auto jObj = json11::Json::object{
            {"type", "Render::Mesh"},
            {"objName", mesh->objName},
            {"defaultColor", defaultColorJArr},
            {"scaler", mesh->scaler},
            {"checkHit", mesh->checkHit}};

        return jObj;
    }
}