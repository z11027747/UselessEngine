
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
		auto &position = transform->position;
		auto &eulerAngles = transform->eulerAngles;

		auto translation = glm::translate(glm::mat4(1.0f), transform->position);
		auto rotation = glm::eulerAngleZXY(
			glm::radians(transform->eulerAngles.z),
			glm::radians(transform->eulerAngles.x),
			glm::radians(transform->eulerAngles.y));
		camera->view = glm::inverse(translation * rotation);

		auto forward = camera->view * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		auto test = camera->view * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		camera->calcPos = position;
		camera->calcEul = eulerAngles;
		camera->calcForward = forward;
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
			// auto projection1 = glm::perspectiveRH_NO(glm::radians(fov), aspect, near, far);
			// auto projection2 = glm::perspectiveRH_ZO(glm::radians(fov), aspect, near, far);
			camera->projection = glm::perspective(glm::radians(fov), aspect, near, far);
		}
		else if (camera->mode == CameraMode::eOrtho)
		{
			float halfWidth = camera->size * 0.5f;
			float halfHeight = halfWidth / aspect;
			camera->projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
		}

		auto test = camera->projection * camera->view * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}