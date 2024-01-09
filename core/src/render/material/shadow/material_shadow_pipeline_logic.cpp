
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_shadow_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialShadowPipelineLogic::SetVertexAttrDescriptions(Context *context,
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
	void MaterialShadowPipelineLogic::SetViewport(Context *context,
												  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;

		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &viewport = stageInfo.viewport;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(currentExtent.width);
		viewport.height = static_cast<float>(currentExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
	}
	void MaterialShadowPipelineLogic::SetRasterizationCreateInfo(Context *context,
																 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
		// rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
		rasterizationStateCreateInfo.depthBiasEnable = true;
		rasterizationStateCreateInfo.depthBiasConstantFactor = 1.2f;
		rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
		rasterizationStateCreateInfo.depthBiasSlopeFactor = 1.5f;
	}
	void MaterialShadowPipelineLogic::SetDepthStencilCreateInfo(Context *context,
																std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
	void MaterialShadowPipelineLogic::SetColorBlendStage(Context *context,
														 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
}