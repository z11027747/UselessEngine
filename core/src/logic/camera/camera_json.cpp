
#include "logic/camera/camera_comp.h"
#include "common/reflection/type_json.h"
#include "context.h"

template <>
std::shared_ptr<void> Common::ComponentJson<Logic::Camera>::From(const json11::Json &j)
{
    auto jObj = j.object_items();

    auto camera = std::make_shared<Logic::Camera>();
    camera->near = (float)jObj["near"].number_value();
    camera->far = (float)jObj["far"].number_value();
    camera->mode = (Logic::CameraMode)jObj["mode"].int_value();
    camera->fov = (float)jObj["fov"].int_value();
    camera->fov = (float)jObj["fov"].int_value();
    camera->size = (float)jObj["size"].int_value();
    camera->passName = jObj["passName"].string_value();

    return camera;
}

template <>
json11::Json Common::ComponentJson<Logic::Camera>::To(std::shared_ptr<void> component)
{
    auto camera = std::static_pointer_cast<Logic::Camera>(component);

    auto jObj = json11::Json::object{
        {"type", Logic::Camera::type},
        {"near", camera->near},
        {"far", camera->far},
        {"mode", (int)camera->mode},
        {"fov", camera->fov},
        {"size", camera->size},
        {"passName", camera->passName}};

    auto j = json11::Json(jObj);
    return j;
}