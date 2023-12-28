
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/swapchain_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/pipeline_layout_logic.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/unit/unit_comp.h"
#include "common/res_system.h"
#include "context.h"
#include "engine_object.h"

namespace Render {

	void PipelineLogic::Create(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		auto& shaderModules = graphicsPipeline->shaderModules;
		auto& shaderStageCreateInfos = graphicsPipeline->shaderStageCreateInfo;
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStageCreateInfos.size());
		pipelineCreateInfo.pStages = shaderStageCreateInfos.data();

		auto& vertexAttrDescriptions = graphicsPipeline->vertexAttrDescriptions;
		VkVertexInputBindingDescription bindingDescription = {};
		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		MakeVertexInputCreateInfo(bindingDescription, vertexAttrDescriptions, vertexInputStateCreateInfo);
		pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
		MakeInputAssemblyCreateInfo(inputAssemblyStateCreateInfo);
		pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;

		auto& surfaceCapabilities = global->surfaceCapabilities;
		VkViewport viewport = {};
		VkRect2D scissor = {};
		VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
		MakeViewportCreateInfo(surfaceCapabilities.currentExtent, viewport, scissor, viewportStateCreateInfo);
		pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;

		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
		MakeRasterizationCreateInfo(rasterizationStateCreateInfo);
		pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;

		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
		MakeMultisampleCreateInfo(multisampleStateCreateInfo);
		pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;

		VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};
		ShaderLogic::MakeDepthStencilCreateInfo(context,
			graphicsPipeline,
			depthStencilStateCreateInfo);
		pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;

		VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
		VkPipelineColorBlendStateCreateInfo colorBlendingStateCreateInfo = {};
		MakeColorBlendCreateInfo(colorBlendAttachmentState, colorBlendingStateCreateInfo);
		pipelineCreateInfo.pColorBlendState = &colorBlendingStateCreateInfo;

		auto& pipelineLayout = graphicsPipeline->pipelineLayout;
		pipelineCreateInfo.layout = pipelineLayout;

		auto& renderPass = global->renderPass;
		pipelineCreateInfo.renderPass = renderPass;
		pipelineCreateInfo.subpass = 0;

		VkPipeline vkPipeline;
		auto ret = vkCreateGraphicsPipelines(logicalDevice, nullptr, 1, &pipelineCreateInfo, nullptr, &vkPipeline);

		graphicsPipeline->pipeline = vkPipeline;
	}

	void PipelineLogic::Destroy(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& pipeline = graphicsPipeline->pipeline;
		vkDestroyPipeline(logicalDevice, pipeline, nullptr);
	}

	void PipelineLogic::MakeVertexInputCreateInfo(
		VkVertexInputBindingDescription& bindingDescription,
		std::vector<VkVertexInputAttributeDescription>& vertexAttrDescriptions,
		VkPipelineVertexInputStateCreateInfo& vertexInputStateCreateInfo
	) {
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;

		bindingDescription.binding = 0;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		bindingDescription.stride = sizeof(Render::Vertex);
		vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescription;

		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttrDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexAttrDescriptions.data();
	}

	void PipelineLogic::MakeInputAssemblyCreateInfo(
		VkPipelineInputAssemblyStateCreateInfo& inputAssemblyStateCreateInfo
	) {
		inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyStateCreateInfo.primitiveRestartEnable = false;
	}

	void PipelineLogic::MakeViewportCreateInfo(
		VkExtent2D& extent,
		VkViewport& viewport, VkRect2D& scissor,
		VkPipelineViewportStateCreateInfo& viewportStateCreateInfo
	) {
		viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = extent.width;
		viewport.height = extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		viewportStateCreateInfo.viewportCount = 1;
		viewportStateCreateInfo.pViewports = &viewport;

		scissor.offset = { 0,0 };
		scissor.extent = extent;
		viewportStateCreateInfo.scissorCount = 1;
		viewportStateCreateInfo.pScissors = &scissor;
	}

	void PipelineLogic::MakeRasterizationCreateInfo(
		VkPipelineRasterizationStateCreateInfo& rasterizationStateCreateInfo
	) {
		rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateCreateInfo.rasterizerDiscardEnable = false;
		rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationStateCreateInfo.lineWidth = 1.0f;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
		rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationStateCreateInfo.depthClampEnable = false;
		rasterizationStateCreateInfo.depthBiasEnable = false;
	}

	void PipelineLogic::MakeMultisampleCreateInfo(
		VkPipelineMultisampleStateCreateInfo& multisampleStateCreateInfo
	) {
		multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleStateCreateInfo.sampleShadingEnable = false;
		multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	}

	void PipelineLogic::MakeColorBlendCreateInfo(
		VkPipelineColorBlendAttachmentState& colorBlendAttachmentState,
		VkPipelineColorBlendStateCreateInfo& colorBlendingStateCreateInfo
	) {
		colorBlendAttachmentState.blendEnable = false;
		colorBlendAttachmentState.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		colorBlendingStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendingStateCreateInfo.logicOpEnable = false;
		colorBlendingStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
		colorBlendingStateCreateInfo.attachmentCount = 1;
		colorBlendingStateCreateInfo.pAttachments = &colorBlendAttachmentState;
	}

}