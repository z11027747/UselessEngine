
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_light_model_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialLightModelPipelineLogic::SetVertexAttrDescriptions(Context *context,
																	std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkVertexInputAttributeDescription positionOSDescription = {
			0, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, positionOS)};

		VkVertexInputAttributeDescription normalOSDescription = {
			1, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, normalOS)};

		VkVertexInputAttributeDescription tangentOSDescription = {
			2, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, tangentOS)};

		VkVertexInputAttributeDescription colorDescription = {
			3, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			offsetof(Render::Vertex, color)};

		VkVertexInputAttributeDescription uv0Description = {
			4, // location
			0, // binding
			VK_FORMAT_R32G32_SFLOAT,
			offsetof(Render::Vertex, uv0)};

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription,
			normalOSDescription,
			tangentOSDescription,
			colorDescription,
			uv0Description};
	}
	void MaterialLightModelPipelineLogic::SetViewport(Context *context,
													  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
	void MaterialLightModelPipelineLogic::SetRasterizationCreateInfo(Context *context,
																	 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;

		// TODO 懒得处理缩放为负数的情况
		//  rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
	}
	void MaterialLightModelPipelineLogic::SetMultisampleCreateInfo(Context *context,
																   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto msaaSamples = global->msaaSamples;

		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &multisampleStateCreateInfo = stageInfo.multisampleStateCreateInfo;
		multisampleStateCreateInfo.rasterizationSamples = msaaSamples;
		multisampleStateCreateInfo.sampleShadingEnable = true;
		multisampleStateCreateInfo.minSampleShading = 0.2f;
	}
	void MaterialLightModelPipelineLogic::SetDepthStencilCreateInfo(Context *context,
																	std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &depthStencilStateCreateInfo = stageInfo.depthStencilStateCreateInfo;
		depthStencilStateCreateInfo.depthTestEnable = true;
		depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilStateCreateInfo.depthWriteEnable = true;
	}
	void MaterialLightModelPipelineLogic::SetColorBlendStage(Context *context,
															 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
}