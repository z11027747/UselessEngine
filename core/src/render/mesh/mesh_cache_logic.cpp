
#include <algorithm>
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "common/res_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MeshLogic::CreateCache(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;

        auto meshCache = std::make_shared<MeshCache>();
        context->AddComponent(globalEO, meshCache);
    }

    void MeshLogic::DestroyCache(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto meshCache = globalEO->GetComponent<MeshCache>();

        auto &sharedMap = meshCache->sharedMap;
        for (const auto &kv : sharedMap)
        {
            auto &sharedInstance = kv.second;
            Destroy(context, sharedInstance);
        }
        sharedMap.clear();

        // TODO 没用的应该及时删除
        auto &deletes = meshCache->deletes;
        for (const auto &instance : deletes)
        {
            Destroy(context, instance);
        }
        deletes.clear();

        context->RemoveComponent<MeshCache>(globalEO);
    }
}