
#include "editor/json/component_json.h"
#include "render/mesh/mesh_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<Render::Mesh> ComponentJson<Render::Mesh>::From(Context *context, const json11::Json &j)
    {
        auto &vertexColorJArr = j["vertexColor"].array_items();
        auto vertexColorR = (float)vertexColorJArr.at(0).number_value();
        auto vertexColorG = (float)vertexColorJArr.at(1).number_value();
        auto vertexColorB = (float)vertexColorJArr.at(2).number_value();

        auto mesh = std::make_shared<Render::Mesh>();
        auto meshInfo = std::make_shared<Render::MeshInfo>();
        meshInfo->objName = j["objName"].string_value();
        meshInfo->vertexColor = glm::vec3(vertexColorR, vertexColorG, vertexColorB);
        meshInfo->checkHit = j["checkHit"].bool_value();
        mesh->info = meshInfo;

        return mesh;
    }

    template <>
    json11::Json ComponentJson<Render::Mesh>::To(Context *context,
                                                 std::shared_ptr<Render::Mesh> mesh)
    {
        auto &meshInfo = mesh->info;
        auto &vertexColor = meshInfo->vertexColor;

        auto vertexColorJArr = json11::Json::array{vertexColor.r, vertexColor.g, vertexColor.b};

        auto jObj = json11::Json::object{
            {"type", Type_Render_Mesh},
            {"objName", meshInfo->objName},
            {"vertexColor", vertexColorJArr},
            {"checkHit", meshInfo->checkHit}};

        return jObj;
    }
}