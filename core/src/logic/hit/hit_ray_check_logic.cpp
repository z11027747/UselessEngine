
#include <string>
#include "render/mesh/mesh_comp.h"
#include "logic/hit/hit_ray_logic.h"
#include "logic/camera/camera_logic.h"
#include "common/log_system.h"
#include "engine_object.h"
#include "context.h"

namespace Logic
{
    std::shared_ptr<EngineObject> HitRayCheckLogic::FromNdc(Context *context,
                                                            float ndcX, float ndcY)
    {
        auto worldPos_near = CameraLogic::TransformNdcToWorld(context, glm::vec3(ndcX, ndcY, 0.0f));
        auto worldPos_far = CameraLogic::TransformNdcToWorld(context, glm::vec3(ndcX, ndcY, 1.0f));

        HitRay ray = {};
        ray.origin = worldPos_near;
        ray.direction = glm::normalize(worldPos_far - worldPos_near);

        auto &logicHitEOs = context->logicHitEOs;
        for (const auto &hitEO : logicHitEOs)
        {
            if (!hitEO->active)
                continue;
            auto result = Test(context,
                               ray, hitEO);
            if (result)
            {
                // Common::LogSystem::Info("Ray Check Target. Name:", hitEO->name);
                return hitEO;
            }
        }

        return nullptr;
    }

    bool HitRayCheckLogic::Test(Context *context,
                                HitRay &ray, std::shared_ptr<EngineObject> hitEO)
    {
        auto hitTransform = hitEO->GetComponent<Transform>();
        auto hitMesh = hitEO->GetComponent<Render::Mesh>();

        auto &bound = hitMesh->bound;
        auto position = bound.center + hitTransform->worldPosition;
        auto radius = bound.radius * hitTransform->localScale.x;

        auto &origin = ray.origin;
        auto &direction = ray.direction;

        auto distPO = glm::distance(position, origin);
        if (distPO <= radius)
        {
            return true;
        }

        auto to = position - origin;
        auto dot = glm::dot(to, direction);
        if (dot < 0.0f)
        {
            return false;
        }

        auto distSq = distPO * distPO - dot * dot;
        if (distSq > radius * radius)
        {
            return false;
        }

        return true;
    }
}