
#include <vulkan/vulkan.h>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_light_mode_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialLightModePipelineLogic::SetVertexAttrDescriptions(Context *context,
																   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkVertexInputAttributeDescription positionOSDescription0 = {
			0, // location
			0,
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, positionOS)};

		VkVertexInputAttributeDescription normalOSDescription1 = {
			1, // location
			0,
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, normalOS)};

		VkVertexInputAttributeDescription tangentOSDescription2 = {
			2, // location
			0,
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, tangentOS)};

		VkVertexInputAttributeDescription colorDescription3 = {
			3, // location
			0,
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, color)};

		VkVertexInputAttributeDescription uv0Description4 = {
			4, // location
			0,
			VK_FORMAT_R32G32_SFLOAT,
			offsetof(Render::Vertex, uv0)};

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription0,
			normalOSDescription1,
			tangentOSDescription2,
			colorDescription3,
			uv0Description4};
	}
	void MaterialLightModePipelineLogic::SetViewport(Context *context,
													 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
	void MaterialLightModePipelineLogic::SetRasterizationCreateInfo(Context *context,
																	std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	}
	void MaterialLightModePipelineLogic::SetDepthStencilCreateInfo(Context *context,
																   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &depthStencilStateCreateInfo = stageInfo.depthStencilStateCreateInfo;
		depthStencilStateCreateInfo.depthTestEnable = true;
		depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilStateCreateInfo.depthWriteEnable = true;
	}
	void MaterialLightModePipelineLogic::SetColorBlendStage(Context *context,
															std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
}