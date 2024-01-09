
#include <vulkan/vulkan.h>
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_color_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialColorPipelineLogic::SetVertexAttrDescriptions(Context *context,
																	std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkVertexInputAttributeDescription positionOSDescription0 = {};
		positionOSDescription0.location = 0;
		positionOSDescription0.binding = 0;
		positionOSDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
		positionOSDescription0.offset = offsetof(Render::Vertex, positionOS);

		VkVertexInputAttributeDescription colorDescription1 = {};
		colorDescription1.location = 1;
		colorDescription1.binding = 0;
		colorDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
		colorDescription1.offset = offsetof(Render::Vertex, color);

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription0,
			colorDescription1};
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
		depthStencilStateCreateInfo.depthTestEnable = true;
		depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_ALWAYS;
		depthStencilStateCreateInfo.depthWriteEnable = false;
	}
	void MaterialColorPipelineLogic::SetColorBlendStage(Context *context,
															 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
}