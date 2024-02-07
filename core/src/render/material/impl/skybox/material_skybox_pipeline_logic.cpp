
#include <vulkan/vulkan.h>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_skybox_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	void MaterialSkyboxPipelineLogic::SetVertexAttrDescriptions(Context *context,
																std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkVertexInputAttributeDescription positionOSDescription = {
			0, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			0};

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription};
	}
	void MaterialSkyboxPipelineLogic::SetRasterizationCreateInfo(Context *context,
																 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
	}
	void MaterialSkyboxPipelineLogic::SetDepthStencilCreateInfo(Context *context,
																std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &depthStencilStateCreateInfo = stageInfo.depthStencilStateCreateInfo;
		depthStencilStateCreateInfo.depthTestEnable = false;
		depthStencilStateCreateInfo.depthWriteEnable = false;
	}
}