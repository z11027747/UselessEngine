
#include "render/material/material_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "common/res_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialLogic::CreateCache(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;

        auto materialCache = std::make_shared<MaterialCache>();
        context->AddComponent(globalEO, materialCache);
    }

    void MaterialLogic::DestroyCache(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto materialCache = globalEO->GetComponent<MaterialCache>();

        auto &sharedImageMap = materialCache->sharedImageMap;
        for (auto &kv : sharedImageMap)
        {
            auto &sharedImage = kv.second;
            ImageLogic::Destroy(context, sharedImage);
        }
        sharedImageMap.clear();

        // TODO 没用的应该及时删除
        auto &deletes = materialCache->deletes;
        for (auto &data : deletes)
        {
            Destroy(context, data);
        }
        deletes.clear();

        auto &globalDataMap = materialCache->globalDataMap;
        for (auto &kv : globalDataMap)
        {
            auto &globalData = kv.second;
            Destroy(context, globalData);
        }
        globalDataMap.clear();

        context->RemoveComponent<MaterialCache>(globalEO);
    }
}