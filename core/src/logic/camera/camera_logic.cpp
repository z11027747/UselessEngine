
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
		auto &position = transform->position;
		auto &eulerAngles = transform->eulerAngles;

		auto quat = glm::quat(glm::radians(eulerAngles));
		auto forward = quat * glm::vec3(0.0f, 0.0f, 1.0f);
		auto up = quat * glm::vec3(0.0f, 1.0f, 0.0f);

		camera->view = glm::lookAt(position, position + forward, up);
		// camera->view[0][0] *= -1;

		camera->calcPos = position;
		camera->calcEul = eulerAngles;
		camera->calcForward = forward;
		camera->calcUp = up;
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

		camera->projection[1][1] *= -1;
	}
}