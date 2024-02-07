
#include "editor/axis/axis_comp.h"
#include "editor/axis/axis_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    void AxisLogic::OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->editorAxisEOs.push_back(eo);
    }
    void AxisLogic::OnRemove(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->RemoveEOInVector(eo, context->editorAxisEOs);
    }
}