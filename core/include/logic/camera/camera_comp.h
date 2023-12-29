#pragma once

#include <glm/glm.hpp>
#include "render/vk/pipeline/pipeline_comp.h"

namespace Logic {

	struct Frustum final {
		float near;
		float far;
		float aspect;
		float fov;
	};

	struct Camera final {
		Frustum frustum;

		glm::mat4 view;
		glm::mat4 projection;
		glm::vec3 calcPos;
		glm::vec3 calcEul;

		VkRenderPass renderPass;

	};

}