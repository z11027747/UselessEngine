
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
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
	void CameraLogic::CreateMain(Context *context)
	{
		auto mainCameraEO = std::make_shared<EngineObject>();
		mainCameraEO->name = G_MainCamera;

		Logic::TransformLogic::Add(mainCameraEO,
								   glm::vec3(0.0f, 6.0f, -10.0f),
								   glm::vec3(32.0f, 0.0f, 0.0f),
								   glm::vec3(1.0f, 1.0f, 1.0f));

		auto mainCamera = std::make_shared<Camera>();
		mainCameraEO->AddComponent<Camera>(mainCamera);

		mainCamera->frustum = {
			0.1f,
			50.0f,
			context->aspect,
			45.0f};
		UpdateView(mainCameraEO);
		UpdateProjection(mainCamera);

		mainCamera->renderPass = context->renderMainPass;

		context->AddEO(mainCameraEO);
	}

	void CameraLogic::DestroyAll(Context *context)
	{
		// nothing
	}

	void CameraLogic::UpdateView(std::shared_ptr<EngineObject> eo)
	{
		auto transform = eo->GetComponent<Transform>();
		auto &position = transform->position;
		auto &eulerAngles = transform->eulerAngles;

		auto rotationMatrix = glm::eulerAngleYXZ(eulerAngles.y, eulerAngles.x, eulerAngles.z);
		auto forward = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
		auto up = glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

		auto camera = eo->GetComponent<Camera>();

		camera->view = glm::lookAt(position, position + forward, up);
		camera->calcPos = position;
		camera->calcEul = eulerAngles;
	}

	void CameraLogic::UpdateProjection(std::shared_ptr<Camera> camera)
	{
		auto near = camera->frustum.near;
		auto far = camera->frustum.far;
		auto aspect = camera->frustum.aspect;
		auto fov = camera->frustum.fov;

		camera->projection = glm::perspective(glm::radians(fov), aspect, near, far);
		camera->projection[1][1] *= -1;
	}
}