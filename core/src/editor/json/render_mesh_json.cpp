
#include "editor/json/component_json.h"
#include "render/mesh/mesh_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<void> ComponentJson<Render::Mesh>::From(const json11::Json &j)
    {
        auto jObj = j.object_items();

        auto &vertexColorJArr = jObj["vertexColor"].array_items();
        auto vertexColorR = (float)vertexColorJArr.at(0).number_value();
        auto vertexColorG = (float)vertexColorJArr.at(1).number_value();
        auto vertexColorB = (float)vertexColorJArr.at(2).number_value();

        auto mesh = std::make_shared<Render::Mesh>();
        auto meshInfo = std::make_shared<Render::MeshInfo>();
        meshInfo->objName = jObj["objName"].string_value();
        meshInfo->vertexColor = glm::vec3(vertexColorR, vertexColorG, vertexColorB);
        meshInfo->checkHit = jObj["checkHit"].bool_value();
        mesh->info = meshInfo;

        return mesh;
    }

    template <>
    json11::Json ComponentJson<Render::Mesh>::To(std::shared_ptr<void> component)
    {
        auto mesh = std::static_pointer_cast<Render::Mesh>(component);

        auto &meshInfo = mesh->info;
        auto &vertexColor = meshInfo->vertexColor;

        auto vertexColorJArr = json11::Json::array{vertexColor.r, vertexColor.g, vertexColor.b};

        auto jObj = json11::Json::object{
            {"type", Render::Mesh::type},
            {"objName", meshInfo->objName},
            {"vertexColor", vertexColorJArr},
            {"checkHit", meshInfo->checkHit}};

        return jObj;
    }
}