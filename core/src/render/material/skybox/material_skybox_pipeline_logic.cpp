
#include <vulkan/vulkan.h>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_skybox_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialSkyboxPipelineLogic::SetVertexAttrDescriptions(Context *context,
																std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkVertexInputAttributeDescription positionOSDescription0 = {};
		positionOSDescription0.location = 0;
		positionOSDescription0.binding = 0;
		positionOSDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
		positionOSDescription0.offset = 0;

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription0};
	}
	void MaterialSkyboxPipelineLogic::SetViewport(Context *context,
												  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
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
		depthStencilStateCreateInfo.depthTestEnable = true;
		depthStencilStateCreateInfo.depthWriteEnable = false;
	}
	void MaterialSkyboxPipelineLogic::SetColorBlendStage(Context *context,
														 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
}