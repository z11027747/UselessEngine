
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/shader_impl/shader_shadow_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "context.h"
#include "engine_object.h"

namespace Render
{
	void ShaderShadowLogic::CreateVertexAttrDescriptions(Context *context,
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

	void ShaderShadowLogic::SetViewport(Context *context,
										std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
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

	void ShaderShadowLogic::SetRasterizationCreateInfo(Context *context,
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
}