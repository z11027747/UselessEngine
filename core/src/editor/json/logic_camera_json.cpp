
#include "editor/json/component_json.h"
#include "logic/camera/camera_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    json11::Json ComponentJson<Logic::Camera>::To(Context *context,
                                                  std::shared_ptr<Logic::Camera> camera)
    {
        auto jObj = json11::Json::object{{"near", camera->near},
                                         {"far", camera->far},
                                         {"aspect", camera->aspect},
                                         {"mode", (int)camera->mode},
                                         {"fov", camera->fov},
                                         {"size", camera->size},
                                         {"passName", camera->passName}};

        return json11::Json::object{{"camera", jObj}};
    }
}