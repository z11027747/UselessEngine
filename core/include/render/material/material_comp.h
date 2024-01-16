#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/image/image_comp.h"

namespace Render
{
	const std::string Pipeline_Skybox = "skybox";
	const std::string Pipeline_Shadow = "shadow";
	const std::string Pipeline_Bling_Phone = "bling_phone";
	const std::string Pipeline_Color = "color";

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

	struct MaterialInstance final
	{
		std::string pipelineName;
		std::vector<std::string> image0Names;

		std::shared_ptr<Image> image0;
		std::shared_ptr<Descriptor> descriptor;
	};

	struct Material final
	{
		std::string pipelineName{Pipeline_Bling_Phone};
		std::vector<std::string> image0Names{"resource/texture/white.png"};
		bool castShadow{false};

		std::shared_ptr<MaterialInstance> instance;
		bool hasChanged;
	};

	struct MaterialInstanceCache final
	{
		std::unordered_map<std::string, std::vector<std::shared_ptr<MaterialInstance>>> sharedMap{};
		std::vector<std::shared_ptr<MaterialInstance>> deletes{};
	};
}
