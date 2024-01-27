
#include "editor/json/component_json.h"
#include "logic/transform/transform_comp.h"
#include "context.h"

namespace Editor
{
    template <>
    std::shared_ptr<void> ComponentJson<Logic::Transform>::From(const json11::Json &j)
    {
        auto jObj = j.object_items();

        auto &localPositionJArr = jObj["localPosition"].array_items();
        auto localPositionX = (float)localPositionJArr.at(0).number_value();
        auto localPositionY = (float)localPositionJArr.at(1).number_value();
        auto localPositionZ = (float)localPositionJArr.at(2).number_value();

        auto &localEulerAnglesJArr = jObj["localEulerAngles"].array_items();
        auto localEulerAnglesX = (float)localEulerAnglesJArr.at(0).number_value();
        auto localEulerAnglesY = (float)localEulerAnglesJArr.at(1).number_value();
        auto localEulerAnglesZ = (float)localEulerAnglesJArr.at(2).number_value();

        auto &localScaleJArr = jObj["localScale"].array_items();
        auto localScaleX = (float)localScaleJArr.at(0).number_value();
        auto localScaleY = (float)localScaleJArr.at(1).number_value();
        auto localScaleZ = (float)localScaleJArr.at(2).number_value();

        auto &childEONameJArr = jObj["childEONames"].array_items();

        auto transform = std::make_shared<Logic::Transform>();
        transform->localPosition = glm::vec3(localPositionX, localPositionY, localPositionZ);
        transform->localEulerAngles = glm::vec3(localEulerAnglesX, localEulerAnglesY, localEulerAnglesZ);
        transform->localScale = glm::vec3(localScaleX, localScaleY, localScaleZ);
        transform->parentEOName = jObj["parentEOName"].string_value();
        for (const auto &childEONameJObj : childEONameJArr)
        {
            transform->childEONames.push_back(childEONameJObj.string_value());
        }

        return transform;
    }

    template <>
    json11::Json ComponentJson<Logic::Transform>::To(std::shared_ptr<void> component)
    {
        auto transform = std::static_pointer_cast<Logic::Transform>(component);

        auto &localPosition = transform->localPosition;
        auto &localEulerAngles = transform->localEulerAngles;
        auto &localScale = transform->localScale;

        auto localPositionJArr = json11::Json::array{localPosition.x, localPosition.y, localPosition.z};
        auto localEulerAnglesJArr = json11::Json::array{localEulerAngles.x, localEulerAngles.y, localEulerAngles.z};
        auto localScaleJArr = json11::Json::array{localScale.x, localScale.y, localScale.z};

        auto jObj = json11::Json::object{
            {"type", Logic::Transform::type},
            {"localPosition", localPositionJArr},
            {"localEulerAngles", localEulerAnglesJArr},
            {"localScale", localScaleJArr},
            {"parentEOName", transform->parentEOName},
            {"childEONames", transform->childEONames}};

        auto j = json11::Json(jObj);
        return j;
    }
}