#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace Logic
{
	enum class CameraMode
	{
		ePerspective = 0,
		eOrtho = 1
	};

	struct Camera final
	{
		inline static std::string type{"Logic::Camera"};

		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 inverseView;
		glm::mat4 inverseProjection;

		float near;
		float far;
		CameraMode mode;
		float fov;
		float size;

		std::string passName;
	};
}