
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "engine_object.hpp"
#include "engine_component.hpp"
#include "context.hpp"

namespace Logic
{
    void ScaleLogic::OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
        auto &cameraEO = context->logicMainCameraEO;
        auto currDistance = TransformLogic::Distance(eo, cameraEO);

        auto scaleKeep = eo->GetComponent<ScaleKeep>();
        scaleKeep->initDistance = currDistance;

        context->logicScaleEOs.push_back(eo);
    }

    void ScaleLogic::OnRemove(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->RemoveEOInVector(eo, context->logicScaleEOs);
    }
}