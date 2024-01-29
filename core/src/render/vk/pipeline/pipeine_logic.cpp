
#include <vector>
#include <array>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/pipeline_layout_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/material_logic.h"
#include "common/res_system.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void PipelineLogic::Create(Context *context,
							   const std::string &name, std::shared_ptr<Pass> pass)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		auto graphicsPipeline = std::make_shared<GraphicsPipeline>();
		graphicsPipeline->name = name;

		CreateShaderStage(context, graphicsPipeline);
		CreateVertexInputStage(context, graphicsPipeline);
		CreateInputAssemblyStage(context, graphicsPipeline);
		CreateViewportStage(context, graphicsPipeline);
		CreateRasterizationStage(context, graphicsPipeline);
		CreateMultisampleStage(context, graphicsPipeline, pass);
		CreateDepthStencilStage(context, graphicsPipeline);
		CreateColorBlendStage(context, graphicsPipeline);

		auto &stageInfo = graphicsPipeline->stageInfo;

		VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.stageCount = static_cast<uint32_t>(stageInfo.shaderStageCreateInfos.size());
		pipelineCreateInfo.pStages = stageInfo.shaderStageCreateInfos.data();
		pipelineCreateInfo.pVertexInputState = &stageInfo.vertexInputStateCreateInfo;
		pipelineCreateInfo.pInputAssemblyState = &stageInfo.inputAssemblyStateCreateInfo;
		pipelineCreateInfo.pViewportState = &stageInfo.viewportStateCreateInfo;
		pipelineCreateInfo.pRasterizationState = &stageInfo.rasterizationStateCreateInfo;
		pipelineCreateInfo.pMultisampleState = &stageInfo.multisampleStateCreateInfo;
		pipelineCreateInfo.pDepthStencilState = &stageInfo.depthStencilStateCreateInfo;
		pipelineCreateInfo.pColorBlendState = &stageInfo.colorBlendingStateCreateInfo;

		MaterialDescriptorLogic::CreateSetLayout(context, graphicsPipeline);
		PipelineLayoutLogic::Create(context, graphicsPipeline);

		auto &pipelineLayout = graphicsPipeline->pipelineLayout;
		pipelineCreateInfo.layout = pipelineLayout;

		auto &renderPass = pass->renderPass;
		pipelineCreateInfo.renderPass = renderPass;
		pipelineCreateInfo.subpass = 0;

		VkPipeline vkPipeline;
		auto ret = vkCreateGraphicsPipelines(logicalDevice, nullptr, 1, &pipelineCreateInfo, nullptr, &vkPipeline);

		graphicsPipeline->pipeline = vkPipeline;

		global->pipelineMap.emplace(graphicsPipeline->name, graphicsPipeline);
	}

	void PipelineLogic::Destroy(Context *context,
								std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		PipelineLayoutLogic::Destroy(context, graphicsPipeline);
		MaterialDescriptorLogic::DestroySetLayout(context, graphicsPipeline);

		DestroyShaderStage(context, graphicsPipeline);

		auto &pipeline = graphicsPipeline->pipeline;
		vkDestroyPipeline(logicalDevice, pipeline, nullptr);
	}

	void PipelineLogic::DestroyAll(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();

		for (const auto &kv : global->pipelineMap)
		{
			auto &pipeline = kv.second;
			Destroy(context, pipeline);
		}
	}

	void PipelineLogic::CreateShaderStage(Context *context,
										  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		auto &name = graphicsPipeline->name;
		auto &stageInfo = graphicsPipeline->stageInfo;

		std::array<VkShaderStageFlagBits, 2> stages = {
			VK_SHADER_STAGE_VERTEX_BIT,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::array<std::vector<char>, 2> codes = {
			Common::ResSystem::ReadAllBytes("resource/spv/" + name + ".vert.spv"),
			Common::ResSystem::ReadAllBytes("resource/spv/" + name + ".frag.spv")};

		for (auto i = 0; i < 2; i++)
		{
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = codes[i].size();
			createInfo.pCode = reinterpret_cast<const uint32_t *>(codes[i].data());

			VkShaderModule shaderModule;
			auto vertRet = vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
			CheckRet(vertRet, "vkCreateShaderModule");

			stageInfo.shaderModules.push_back(shaderModule);

			VkPipelineShaderStageCreateInfo stageCreateInfo = {};
			stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageCreateInfo.stage = stages[i];
			stageCreateInfo.module = shaderModule;
			stageCreateInfo.pName = "main";

			stageInfo.shaderStageCreateInfos.push_back(stageCreateInfo);
		}
	}

	void PipelineLogic::DestroyShaderStage(Context *context,
										   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;
		auto &shaderModules = stageInfo.shaderModules;
		for (const auto &shaderModule : shaderModules)
		{
			vkDestroyShaderModule(LogicalDeviceLogic::Get(context),
								  shaderModule, nullptr);
		}

		stageInfo.shaderModules.clear();
		stageInfo.shaderStageCreateInfos.clear();
	}

	void PipelineLogic::CreateVertexInputStage(Context *context,
											   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &vertexInputAttributeDescriptions = stageInfo.vertexInputAttributeDescriptions;
		MaterialPipelineLogic::SetVertexAttrDescriptions(context, graphicsPipeline);

		auto &vertexBindingDescription = stageInfo.vertexBindingDescription;
		vertexBindingDescription.binding = 0;
		vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		vertexBindingDescription.stride = sizeof(Render::Vertex);

		auto &vertexInputStateCreateInfo = stageInfo.vertexInputStateCreateInfo;
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
		vertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexBindingDescription;
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributeDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data();
	}

	void PipelineLogic::CreateInputAssemblyStage(Context *context,
												 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &inputAssemblyStateCreateInfo = stageInfo.inputAssemblyStateCreateInfo;
		inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyStateCreateInfo.primitiveRestartEnable = false;
	}

	void PipelineLogic::CreateViewportStage(Context *context,
											std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;

		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &viewport = stageInfo.viewport;
		viewport.x = 0.0f;
		viewport.y = static_cast<float>(currentExtent.height);
		viewport.width = static_cast<float>(currentExtent.width);
		viewport.height = -static_cast<float>(currentExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		MaterialPipelineLogic::SetViewport(context, graphicsPipeline);

		auto &scissor = stageInfo.scissor;
		scissor.offset = {0, 0};
		scissor.extent = currentExtent;

		auto &viewportStateCreateInfo = stageInfo.viewportStateCreateInfo;
		viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateCreateInfo.viewportCount = 1;
		viewportStateCreateInfo.pViewports = &viewport;
		viewportStateCreateInfo.scissorCount = 1;
		viewportStateCreateInfo.pScissors = &scissor;
	}

	void PipelineLogic::CreateRasterizationStage(Context *context,
												 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
		rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateCreateInfo.rasterizerDiscardEnable = false;
		rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationStateCreateInfo.lineWidth = 1.0f;
		// rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_LINE;
		// rasterizationStateCreateInfo.lineWidth = 2.0f;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
		rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationStateCreateInfo.depthClampEnable = false;
		rasterizationStateCreateInfo.depthBiasEnable = false;

		MaterialPipelineLogic::SetRasterizationCreateInfo(context, graphicsPipeline);
	}

	void PipelineLogic::CreateMultisampleStage(Context *context,
											   std::shared_ptr<GraphicsPipeline> graphicsPipeline,
											   std::shared_ptr<Pass> pass)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &multisampleStateCreateInfo = stageInfo.multisampleStateCreateInfo;
		multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleStateCreateInfo.rasterizationSamples = pass->msaaSamples;
		multisampleStateCreateInfo.sampleShadingEnable = false;

		MaterialPipelineLogic::SetMultisampleCreateInfo(context, graphicsPipeline);
	}

	void PipelineLogic::CreateDepthStencilStage(Context *context,
												std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &depthStencilStateCreateInfo = stageInfo.depthStencilStateCreateInfo;
		depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilStateCreateInfo.depthTestEnable = true;
		depthStencilStateCreateInfo.depthWriteEnable = true;
		depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilStateCreateInfo.depthBoundsTestEnable = false;
		depthStencilStateCreateInfo.stencilTestEnable = false;
		depthStencilStateCreateInfo.minDepthBounds = 0.0f;
		depthStencilStateCreateInfo.maxDepthBounds = 1.0f;

		MaterialPipelineLogic::SetDepthStencilCreateInfo(context, graphicsPipeline);
	}

	void PipelineLogic::CreateColorBlendStage(Context *context,
											  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &colorBlendAttachmentState = stageInfo.colorBlendAttachmentState;
		colorBlendAttachmentState.blendEnable = false;
		colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
												   VK_COLOR_COMPONENT_G_BIT |
												   VK_COLOR_COMPONENT_B_BIT |
												   VK_COLOR_COMPONENT_A_BIT;

		auto &colorBlendingStateCreateInfo = stageInfo.colorBlendingStateCreateInfo;
		colorBlendingStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendingStateCreateInfo.logicOpEnable = false;
		colorBlendingStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
		colorBlendingStateCreateInfo.attachmentCount = 1;
		colorBlendingStateCreateInfo.pAttachments = &colorBlendAttachmentState;

		MaterialPipelineLogic::SetColorBlendStage(context, graphicsPipeline);
	}

}