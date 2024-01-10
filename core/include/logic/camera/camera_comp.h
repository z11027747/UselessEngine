#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

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

		glm::mat4 inverseView;
		glm::mat4 inverseProjection;

		float near;
		float far;
		float aspect;

		CameraMode mode;
		float fov;
		float size;

		std::string passName;
	};
}