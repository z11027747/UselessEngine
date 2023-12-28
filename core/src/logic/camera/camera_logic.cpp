
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "logic/transform/transform_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "context.h"
#include "engine_object.h"

namespace Logic {

	void CameraLogic::Add(std::shared_ptr<EngineObject> eo, Frustum frustum) {
		auto camera = std::make_shared<Camera>();
		camera->frustum = frustum;
		eo->AddComponent<Camera>(camera);

		UpdateView(eo);
		UpdateProjection(camera);
	}

	void CameraLogic::UpdateView(std::shared_ptr<EngineObject> eo) {
		auto transform = eo->GetComponent<Transform>();
		auto& position = transform->position;
		auto& eulerAngles = transform->eulerAngles;

		auto rotationMatrix = glm::eulerAngleYXZ(eulerAngles.y, eulerAngles.x, eulerAngles.z);
		auto forward = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
		auto up = glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

		auto camera = eo->GetComponent<Camera>();

		camera->view = glm::lookAt(position, position + forward, up);
		camera->calcPos = position;
		camera->calcEul = eulerAngles;
	}

	void CameraLogic::UpdateProjection(std::shared_ptr<Camera> camera) {
		auto near = camera->frustum.near;
		auto far = camera->frustum.far;
		auto aspect = camera->frustum.aspect;
		auto fov = camera->frustum.fov;

		camera->projection = glm::perspective(glm::radians(fov), aspect, near, far);
		camera->projection[1][1] *= -1;
	}

}