
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MeshLogic::OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->renderMeshEOs.push_back(eo);
    }

    void MeshLogic::OnRemove(Context *context, std::shared_ptr<EngineObject> eo)
    {
        TryDestroyEO(context, eo);
        
        context->RemoveEOInVector(eo, context->renderMeshEOs);
    }
}