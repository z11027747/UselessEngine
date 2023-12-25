#pragma once

#include <glm/glm.hpp>

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

	};

}
