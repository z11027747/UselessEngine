
#include "editor/json/component_json.h"
#include "logic/camera/camera_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<Logic::Camera> ComponentJson<Logic::Camera>::From(Context *context, const json11::Json &j)
    {
        auto camera = std::make_shared<Logic::Camera>();
        camera->near = (float)j["near"].number_value();
        camera->far = (float)j["far"].number_value();
        camera->mode = (Logic::CameraMode)j["mode"].int_value();
        camera->fov = (float)j["fov"].int_value();
        camera->fov = (float)j["fov"].int_value();
        camera->size = (float)j["size"].int_value();
        camera->passName = j["passName"].string_value();

        return camera;
    }

    template <>
    json11::Json ComponentJson<Logic::Camera>::To(Context *context,
                                                  std::shared_ptr<Logic::Camera> camera)
    {
        auto jObj = json11::Json::object{
            {"type", Type_Logic_Camera},
            {"near", camera->near},
            {"far", camera->far},
            {"mode", (int)camera->mode},
            {"fov", camera->fov},
            {"size", camera->size},
            {"passName", camera->passName}};

        return jObj;
    }
}