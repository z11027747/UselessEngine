
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    bool MeshLogic::IsShared(Context *context,
                             std::shared_ptr<Mesh> mesh)
    {
        return (mesh->vertexColor == glm::vec3(1.0f));
    }

    void MeshLogic::TryDestroyEO(Context *context,
                                 std::shared_ptr<EngineObject> eo)
    {
        auto mesh = eo->GetComponent<Render::Mesh>();
        if (mesh->vertexColor != glm::vec3(1.0f))
        {
            MeshInstanceLogic::SetDestroy(context, mesh->instance);
        }
    }
}