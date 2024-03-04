#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/image/image_comp.h"
#include "define.hpp"

namespace Render
{
	struct CameraUBO final
	{
		glm::vec4 pos;
		glm::mat4 view;
		glm::mat4 projection;
	};

	struct DirectionLightUBO final
	{
		glm::vec4 dir;
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec4 ambient;
		glm::vec4 col;
	};

	struct PointLightUBO final
	{
		glm::vec4 pos;
		// glm::mat4 view;
		// glm::mat4 projection;
		glm::vec4 col;
		glm::vec4 clq;
	};

	struct SpotLightUBO final
	{
		glm::vec4 dir;
		glm::vec4 pos;
		// glm::mat4 view;
		// glm::mat4 projection;
		glm::vec4 col;
		glm::vec4 cutOff;
	};

	struct GlobalUBO final
	{
		CameraUBO camera;
		DirectionLightUBO directionLight;
		PointLightUBO pointLights[256];
		int activePointLights;
		SpotLightUBO spotLights[4];
		int activeSpotLights;
		float currTime;
	};

	struct MaterialUBO final
	{
		glm::vec4 params0;
		glm::vec4 params1;
	};

	struct MaterialInfo final
	{
		std::string pipelineName{Define::Pipeline::Color};
		std::vector<std::string> imageNames{};
		std::vector<glm::vec4> params{{glm::vec4(1.0f)}};
		bool isImageCube{false};
		bool castShadow{false};
		int renderQueue{Define::RenderQueue::Geometry};

		bool hasChanged{false};
	};

	struct MaterialData final
	{
		int id;
		std::shared_ptr<MaterialInfo> info{nullptr};  // ref
		std::vector<std::shared_ptr<Image>> images{}; // ref(shared)
		std::shared_ptr<Buffer> buffer;
		std::shared_ptr<Descriptor> descriptor;
	};

	struct Material final
	{
		inline static std::string type{"Render::Material"};

		std::shared_ptr<MaterialInfo> info{std::make_shared<MaterialInfo>()};
		std::shared_ptr<MaterialData> data;

		bool useInstance{false};
	};

	struct MaterialCache final
	{
		inline static std::string type{"Render::MaterialCache"};

		std::unordered_map<std::string, std::shared_ptr<Image>> sharedImageMap{};
		std::vector<std::shared_ptr<MaterialData>> deletes{};

		std::unordered_map<std::string, std::shared_ptr<MaterialData>> globalDataMap{};

		// instance
		std::unordered_map<std::string, std::shared_ptr<MaterialData>> instanceDataMap{};
	};
}
