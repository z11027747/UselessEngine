#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "render/vk/pass/pass_comp.h"

namespace Logic
{
	enum class CameraMode
	{
		ePerspective = 0,
		eOrtho = 1
	};

	struct Camera final
	{
		glm::mat4 view;
		glm::mat4 projection;

		float near;
		float far;
		float aspect;

		CameraMode mode;
		float fov;
		float size;

		std::shared_ptr<Render::Pass> renderPass;
	};
}