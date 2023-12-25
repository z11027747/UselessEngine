
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/logical_device_system.h"
#include "render/global/physical_device_system.h"
#include "render/global/swapchain_system.h"
#include "render/pipeline/pipeline_comp.h"
#include "render/pipeline/pipeline_system.h"
#include "render/pipeline/descriptor_layout_system.h"
#include "render/pipeline/pipeline_layout_system.h"
#include "render/pipeline/shader_system.h"
#include "common/res_system.h"
#include "context.h"
#include "base.h"

namespace Render {

	void PipelineSystem::Create(Context* context,
		const std::string& name
	) {
		auto pipeline = std::make_shared<Pipeline>();

		DescriptorLayoutSystem::Create(context, pipeline);
		ShaderSystem::Create(context, name, pipeline);
		PipelineLayoutSystem::Create(context, pipeline);

		GreateGraphicsPipeline(context, pipeline);

		auto pipelineEO = std::make_shared<EngineObject>();
		pipelineEO->AddComponent(pipeline);

		context->renderPipelineEOs.emplace(name, pipelineEO);
	}

	void PipelineSystem::DestroyAll(Context* context) {

		auto& renderPipelineEOs = context->renderPipelineEOs;
		for (auto& kv : renderPipelineEOs) {
			auto& name = kv.first;
			auto& renderPipelineEO = kv.second;

			auto pipeline = renderPipelineEO->GetComponent<Pipeline>();

			PipelineLayoutSystem::Destroy(context, pipeline);
			ShaderSystem::Destroy(context, pipeline);
			DescriptorLayoutSystem::Destroy(context, pipeline);

			DestroyGraphicsPipeline(context, pipeline);

			renderPipelineEO->RemoveComponent<Pipeline>();
		}

		context->renderPipelineEOs.clear();
	}

	void PipelineSystem::GreateGraphicsPipeline(Context* context,
		std::shared_ptr<Pipeline> pipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		auto& shader = pipeline->shader;
		auto& shaderModules = shader->modules;
		auto& shaderStageCreateInfos = shader->stageCreateInfo;
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStageCreateInfos.size());
		pipelineCreateInfo.pStages = shaderStageCreateInfos.data();

		VkVertexInputBindingDescription bindingDescription = {};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		MakeVertexInputCreateInfo(bindingDescription, attributeDescriptions, vertexInputStateCreateInfo);
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
		MakeDepthStencilCreateInfo(depthStencilStateCreateInfo);
		pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;

		VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
		VkPipelineColorBlendStateCreateInfo colorBlendingStateCreateInfo = {};
		MakeColorBlendCreateInfo(colorBlendAttachmentState, colorBlendingStateCreateInfo);
		pipelineCreateInfo.pColorBlendState = &colorBlendingStateCreateInfo;

		auto& pipelineLayout = pipeline->pipelineLayout;
		pipelineCreateInfo.layout = pipelineLayout;

		auto& renderPass = global->renderPass;
		pipelineCreateInfo.renderPass = renderPass;
		pipelineCreateInfo.subpass = 0;

		VkPipeline vkPipeline;
		auto ret = vkCreateGraphicsPipelines(logicalDevice, nullptr, 1, &pipelineCreateInfo, nullptr, &vkPipeline);

		pipeline->vkPipeline = vkPipeline;
	}

	void PipelineSystem::DestroyGraphicsPipeline(Context* context,
		std::shared_ptr<Pipeline> pipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		vkDestroyPipeline(logicalDevice, pipeline->vkPipeline, nullptr);
	}

	void PipelineSystem::MakeVertexInputCreateInfo(
		VkVertexInputBindingDescription& bindingDescription,
		std::vector<VkVertexInputAttributeDescription>& attributeDescriptions,
		VkPipelineVertexInputStateCreateInfo& vertexInputStateCreateInfo
	) {
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;

		bindingDescription.binding = 0;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		bindingDescription.stride = sizeof(Render::Vertex);
		vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescription;

		VkVertexInputAttributeDescription positionOSDescription0 = {};
		positionOSDescription0.location = 0;
		positionOSDescription0.binding = 0;
		positionOSDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
		positionOSDescription0.offset = offsetof(Render::Vertex, positionOS);
		attributeDescriptions.push_back(positionOSDescription0);

		VkVertexInputAttributeDescription colorDescription1 = {};
		colorDescription1.location = 1;
		colorDescription1.binding = 0;
		colorDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
		colorDescription1.offset = offsetof(Render::Vertex, color);
		attributeDescriptions.push_back(colorDescription1);

		VkVertexInputAttributeDescription uv0Description2 = {};
		uv0Description2.location = 2;
		uv0Description2.binding = 0;
		uv0Description2.format = VK_FORMAT_R32G32B32_SFLOAT;
		uv0Description2.offset = offsetof(Render::Vertex, uv0);
		attributeDescriptions.push_back(uv0Description2);

		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	}

	void PipelineSystem::MakeInputAssemblyCreateInfo(
		VkPipelineInputAssemblyStateCreateInfo& inputAssemblyStateCreateInfo
	) {
		inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyStateCreateInfo.primitiveRestartEnable = false;
	}

	void PipelineSystem::MakeViewportCreateInfo(
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

	void PipelineSystem::MakeRasterizationCreateInfo(
		VkPipelineRasterizationStateCreateInfo& rasterizationStateCreateInfo
	)
	{
		rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateCreateInfo.rasterizerDiscardEnable = false;
		rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationStateCreateInfo.lineWidth = 1.0f;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationStateCreateInfo.depthClampEnable = false;
		rasterizationStateCreateInfo.depthBiasEnable = false;
	}

	void PipelineSystem::MakeDepthStencilCreateInfo(
		VkPipelineDepthStencilStateCreateInfo& depthStencilStateCreateInfo
	) {
		depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilStateCreateInfo.depthTestEnable = true;
		depthStencilStateCreateInfo.depthWriteEnable = true;
		depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilStateCreateInfo.depthBoundsTestEnable = false;
		depthStencilStateCreateInfo.stencilTestEnable = false;
	}

	void PipelineSystem::MakeMultisampleCreateInfo(
		VkPipelineMultisampleStateCreateInfo& multisampleStateCreateInfo
	) {
		multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleStateCreateInfo.sampleShadingEnable = false;
		multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	}

	void PipelineSystem::MakeColorBlendCreateInfo(
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