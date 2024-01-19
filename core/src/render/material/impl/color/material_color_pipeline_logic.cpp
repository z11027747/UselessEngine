
#include <vulkan/vulkan.h>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_color_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialColorPipelineLogic::SetVertexAttrDescriptions(Context *context,
															   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkVertexInputAttributeDescription positionOSDescription = {
			0, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, positionOS)};

		VkVertexInputAttributeDescription colorDescription = {
			1, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, color)};

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription,
			colorDescription};
	}
	void MaterialColorPipelineLogic::SetViewport(Context *context,
												 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
	void MaterialColorPipelineLogic::SetRasterizationCreateInfo(Context *context,
																std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	}
	void MaterialColorPipelineLogic::SetDepthStencilCreateInfo(Context *context,
															   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &depthStencilStateCreateInfo = stageInfo.depthStencilStateCreateInfo;
		depthStencilStateCreateInfo.depthTestEnable = false;
		// depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilStateCreateInfo.depthWriteEnable = false;
	}
	void MaterialColorPipelineLogic::SetColorBlendStage(Context *context,
														std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
}