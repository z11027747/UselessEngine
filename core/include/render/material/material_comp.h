#pragma once

#include <memory>
#include <string>
#include <vector>
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/image/image_comp.h"

namespace Render
{
	struct Material final
	{
		std::string pipelineName;
		std::vector<std::string> image0Names;
		bool castShadow;

		std::shared_ptr<Image> image0;
		std::shared_ptr<Descriptor> descriptor;
	};

	struct CameraUBO final
	{
		alignas(16) glm::vec3 pos;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
	};

	struct DirectionLightUBO final
	{
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
		alignas(16) glm::vec3 dir;
		alignas(16) glm::vec3 col;
		alignas(16) glm::vec4 params;
	};

	struct GlobalUBO final
	{
		alignas(16) CameraUBO camera;
		alignas(16) DirectionLightUBO directionLight;
	};
}
