
#include <vulkan/vulkan.h>
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_bling_phone_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    void MaterialBlingPhonePipelineLogic::SetVertexAttrDescriptions(Context *context,
														   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
		VkVertexInputAttributeDescription positionOSDescription0 = {};
		positionOSDescription0.location = 0;
		positionOSDescription0.binding = 0;
		positionOSDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
		positionOSDescription0.offset = offsetof(Render::Vertex, positionOS);

		VkVertexInputAttributeDescription normalOSDescription1 = {};
		normalOSDescription1.location = 1;
		normalOSDescription1.binding = 0;
		normalOSDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
		normalOSDescription1.offset = offsetof(Render::Vertex, normalOS);

		VkVertexInputAttributeDescription colorDescription2 = {};
		colorDescription2.location = 2;
		colorDescription2.binding = 0;
		colorDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
		colorDescription2.offset = offsetof(Render::Vertex, color);

		VkVertexInputAttributeDescription uv0Description3 = {};
		uv0Description3.location = 3;
		uv0Description3.binding = 0;
		uv0Description3.format = VK_FORMAT_R32G32_SFLOAT;
		uv0Description3.offset = offsetof(Render::Vertex, uv0);

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription0,
			normalOSDescription1,
			colorDescription2,
			uv0Description3};
    }
    void MaterialBlingPhonePipelineLogic::SetViewport(Context *context,
														   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
    }
    void MaterialBlingPhonePipelineLogic::SetRasterizationCreateInfo(Context *context,
														   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    }
    void MaterialBlingPhonePipelineLogic::SetDepthStencilCreateInfo(Context *context,
														   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
    }
    void MaterialBlingPhonePipelineLogic::SetColorBlendStage(Context *context,
														   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
    }
}