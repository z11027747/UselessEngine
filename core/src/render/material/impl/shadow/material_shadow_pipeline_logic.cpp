
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
		VkVertexInputAttributeDescription positionOSDescription = {
			0, // location
			0, // binding
			VK_FORMAT_R32G32B32_SFLOAT,
			0};

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription};
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
		rasterizationStateCreateInfo.depthBiasConstantFactor = 1.5f;
		rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
		rasterizationStateCreateInfo.depthBiasSlopeFactor = 4.0f;
	}
}