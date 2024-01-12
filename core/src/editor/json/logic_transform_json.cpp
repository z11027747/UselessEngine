
#include "editor/json/component_json.h"
#include "logic/transform/transform_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    json11::Json ComponentJson<Logic::Transform>::To(Context *context,
                                                     std::shared_ptr<Logic::Transform> transform)

    {
        auto &localPosition = transform->localPosition;
        auto &localEulerAngles = transform->localEulerAngles;
        auto &localScale = transform->localScale;

        auto jObj = json11::Json::object{{"localPosition", json11::Json::array{localPosition.x, localPosition.y, localPosition.z}},
                                         {"localEulerAngles", json11::Json::array{localEulerAngles.x, localEulerAngles.y, localEulerAngles.z}},
                                         {"localScale", json11::Json::array{localScale.x, localScale.y, localScale.z}}};

        return json11::Json::object{{"transform", jObj}};
    }
}