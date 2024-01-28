
#include "editor/axis/axis_comp.h"
#include "common/reflection/type_json.h"
#include "context.h"

template <>
std::shared_ptr<void> Common::ComponentJson<Editor::Axis>::From(const json11::Json &j)
{
    auto axis = std::make_shared<Editor::Axis>();

    return axis;
}

template <>
json11::Json Common::ComponentJson<Editor::Axis>::To(std::shared_ptr<void> component)
{
    auto axis = std::static_pointer_cast<Editor::Axis>(component);

    auto jObj = json11::Json::object{
        {"type", Editor::Axis::type}};
    auto j = json11::Json(jObj);
    return j;
}