
#include <cmath>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "context.h"
#include "engine_object.h"

namespace Logic
{
	void CameraLogic::UpdateView(std::shared_ptr<EngineObject> eo)
	{
		auto camera = eo->GetComponent<Camera>();

		auto transform = eo->GetComponent<Transform>();
		auto position = transform->position;
		auto forward = transform->forward;
		auto up = transform->up;

		camera->view = glm::lookAtLH(position, position + forward, up);
	}

	void CameraLogic::UpdateProjection(Context *context,
									   std::shared_ptr<Camera> camera)
	{
		auto aspect = context->aspect;

		auto near = camera->near;
		auto far = camera->far;

		if (camera->mode == CameraMode::ePerspective)
		{
			auto fov = camera->fov;
			camera->projection = glm::perspective(glm::radians(fov), aspect, near, far);
		}
		else if (camera->mode == CameraMode::eOrtho)
		{
			float halfWidth = camera->size * 0.5f;
			float halfHeight = halfWidth / aspect;
			camera->projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
		}
	}
}