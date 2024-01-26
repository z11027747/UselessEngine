#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/buffer/buffer_comp.h"

namespace Render
{
	const std::string Pipeline_Skybox = "skybox";
	const std::string Pipeline_Shadow = "shadow";
	const std::string Pipeline_LightModel = "light_model";
	const std::string Pipeline_Color = "color";

	const int Queue_Skybox = 1000;
	const int Queue_Geometry = 2000;

	struct CameraUBO final
	{
		alignas(16) glm::vec3 pos;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
	};

	struct DirectionLightUBO final
	{
		alignas(16) glm::vec3 dir;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
		alignas(16) glm::vec3 ambient;
		alignas(16) glm::vec3 col;
	};

	struct PointLightUBO final
	{
		alignas(16) glm::vec3 pos;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
		alignas(16) glm::vec3 col;
		alignas(16) glm::vec3 clq;
	};

	struct GlobalUBO final
	{
		alignas(16) CameraUBO camera;
		alignas(16) DirectionLightUBO directionLight;
		alignas(16) PointLightUBO pointLights[4];
		alignas(16) int activePointLights;
	};

	struct MaterialUBO final
	{
		glm::vec4 params;
	};

	struct MaterialInfo final
	{
		std::string pipelineName{Pipeline_Color};
		std::vector<std::string> imageNames{};
		std::vector<glm::vec4> params{{glm::vec4(1.0f)}};
		bool isImageCube{false};
		bool castShadow{false};
		int renderQueue{Queue_Geometry};

		bool hasChanged{false};
	};

	struct MaterialInstance final
	{
		int id;
		std::shared_ptr<MaterialInfo> info{nullptr};  // ref
		std::vector<std::shared_ptr<Image>> images{}; // ref(shared)
		std::shared_ptr<Buffer> buffer;
		std::shared_ptr<Descriptor> descriptor;
	};

	struct Material final
	{
		std::shared_ptr<MaterialInfo> info{std::make_shared<MaterialInfo>()};
		std::shared_ptr<MaterialInstance> instance;
	};

	struct MaterialInstanceCache final
	{
		std::unordered_map<std::string, std::shared_ptr<Image>> sharedImageMap{};
		std::vector<std::shared_ptr<MaterialInstance>> deletes{};
	};
}
