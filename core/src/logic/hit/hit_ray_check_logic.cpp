
#include <string>
#include <limits>
#include "render/mesh/mesh_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/hit/hit_ray_logic.h"
#include "logic/camera/camera_logic.h"
#include "common/log.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    HitRay HitRayCheckLogic::CalcaRayFromNdc(Context *context,
                                             float ndcX, float ndcY)
    {
        auto worldPos_near = CameraLogic::TransformNdcToWorld(context, glm::vec3(ndcX, ndcY, 0.0f));
        auto worldPos_far = CameraLogic::TransformNdcToWorld(context, glm::vec3(ndcX, ndcY, 1.0f));

        HitRay ray = {};
        ray.origin = worldPos_near;
        ray.direction = glm::normalize(worldPos_far - worldPos_near);

        return ray;
    }

    std::shared_ptr<EngineObject> HitRayCheckLogic::TestFromNdc(Context *context,
                                                                float ndcX, float ndcY)
    {
        auto ray = CalcaRayFromNdc(context, ndcX, ndcY);

        std::vector<std::shared_ptr<EngineObject>> tempHitEOs = {};

        auto &meshEOs = context->renderMeshEOs;
        for (const auto &meshEO : meshEOs)
        {
            if (!meshEO->active)
                continue;

            auto mesh = meshEO->GetComponent<Render::Mesh>();
            if (!mesh->info->checkHit)
                continue;

            auto result = Test(context,
                               ray, meshEO);
            if (result)
                tempHitEOs.push_back(meshEO);
        }

        if (tempHitEOs.size() == 0)
            return nullptr;

        std::shared_ptr<EngineObject> minDistHitEO = nullptr;
        auto minDist = 99999.0f;

        for (const auto &hitEO : tempHitEOs)
        {
            auto hitTransform = hitEO->GetComponent<Transform>();
            auto position = hitTransform->worldPosition;
            auto &origin = ray.origin;

            auto dist = glm::distance(position, origin);
            if (minDist > dist)
            {
                minDist = dist;
                minDistHitEO = hitEO;
            }
        }

        return minDistHitEO;
    }

    bool HitRayCheckLogic::Test(Context *context,
                                HitRay &ray, std::shared_ptr<EngineObject> hitEO)
    {
        auto hitTransform = hitEO->GetComponent<Transform>();
        auto hitMesh = hitEO->GetComponent<Render::Mesh>();

        auto &meshInstance = hitMesh->instance;
        auto &boundingSphere = meshInstance->boundingSphere;
        auto position = boundingSphere.center + hitTransform->worldPosition;
        auto radius = boundingSphere.radius * hitTransform->worldScale.x;

        auto &origin = ray.origin;
        auto &direction = ray.direction;

        auto dist = glm::distance(position, origin);
        if (dist <= radius)
        {
            return true;
        }

        auto to = position - origin;
        auto dot = glm::dot(to, direction);
        if (dot < 0.0f)
        {
            return false;
        }

        auto distSq = dist * dist - dot * dot;
        if (distSq > radius * radius)
        {
            return false;
        }

        return true;
    }
}