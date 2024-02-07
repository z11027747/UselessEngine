
#include <cmath>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include "render/vk/pass/pass_logic.h"
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_logic.h"
#include "context.hpp"
#include "engine_object.hpp"

namespace Logic
{
	void CameraLogic::UpdateView(std::shared_ptr<EngineObject> eo)
	{
		auto camera = eo->GetComponent<Camera>();

		auto transform = eo->GetComponent<Transform>();
		auto &p = transform->localPosition;
		auto &r = transform->right;
		auto &u = transform->up;
		auto &f = transform->forward;

		auto view = glm::mat4(1.0f);
		view[0][0] = r.x;
		view[1][0] = r.y;
		view[2][0] = r.z;
		view[0][1] = u.x;
		view[1][1] = u.y;
		view[2][1] = u.z;
		view[0][2] = -f.x;
		view[1][2] = -f.y;
		view[2][2] = -f.z;
		view[3][0] = -glm::dot(r, p);
		view[3][1] = -glm::dot(u, p);
		view[3][2] = glm::dot(f, p);
		camera->view = view;
		camera->inverseView = glm::inverse(view);
	}

	void CameraLogic::UpdateProjection(Context *context,
									   std::shared_ptr<Camera> camera)
	{
		auto aspect = context->aspect;

		auto near = camera->near;
		auto far = camera->far;

		auto projection = glm::mat4(1.0f);
		if (camera->mode == CameraMode::ePerspective)
		{
			auto fov = glm::radians(camera->fov);
			projection = glm::perspective(fov, aspect, near, far);
		}
		else if (camera->mode == CameraMode::eOrtho)
		{
			float halfWidth = camera->size * 0.5f;
			float halfHeight = halfWidth / aspect;
			projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
		}

		camera->projection = projection;
		camera->inverseProjection = glm::inverse(projection);
	}

	glm::vec3 CameraLogic::TransformNdcToWorld(Context *context,
											   const glm::vec3 &ndcPos)
	{
		auto &mainCameraEO = context->logicMainCameraEO;
		auto mainCamera = mainCameraEO->GetComponent<Camera>();

		auto inverseVP = mainCamera->inverseView * mainCamera->inverseProjection;

		auto ndcPos4 = glm::vec4(ndcPos, 1.0f);
		auto worldPos = inverseVP * ndcPos4;
		worldPos /= worldPos.w;

		return worldPos;
	}
}