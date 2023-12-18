
#include "render/comp.h"
#include "context.h"

//图形管线
//	将提交的顶点和纹理转换为渲染目标上的像素的操作



//之前的许多图形API会为管线提供一些默认的状态
//	在Vulkan不存在默认状态，所有状态必须被显式地设置，
//	无论是视口大小，还是使用的颜色混合函数都需要显式地指定
void RenderSystem::CreateGraphicsPipeline(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	//着色器阶段
	auto& shaderModules = globalInfoComp->shaderModules;
	auto shaderStageCreateInfos = GetShaderModuleCreateInfos(shaderModules);
	pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStageCreateInfos.size());
	pipelineCreateInfo.pStages = shaderStageCreateInfos.data();

	//固定功能阶段
	//顶点输入
	auto vertexInputStateCreateInfo = GetVertexInputCreateInfo();
	pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;

	//输入装配
	//	顶点数据定义了哪种类型的几何图元，以及是否启用几何图元重启
	// 
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
	inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

	//VkPrimitiveTopology 图元类型
	//	VK_PRIMITIVE_TOPOLOGY_POINT_LIST：点图元
	//	VK_PRIMITIVE_TOPOLOGY_LINE_LIST：每两个顶点构成一个线段图元
	//	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST：每三个顶点构成一个三角形图元
	inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyStateCreateInfo.primitiveRestartEnable = false;

	//视口+裁剪
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

	//视口
	auto& swapChainExtent = globalInfoComp->swapChainExtent;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = swapChainExtent.width;
	viewport.height = swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;

	//裁剪
	VkRect2D scissor = {};
	scissor.offset = { 0,0 };
	scissor.extent = swapChainExtent;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	//光栅化
	//	来顶点着色器的顶点构成的几何图元转换为片段交由片段着色器着色
	//	深度测试，背面剔除和裁剪测试如何开启了，也由光栅化程序执行
	// 
	//可以配置光栅化程序输出整个几何图元作为片段，还是只输出几何图元的边作为片段(也就是线框模式)

	VkPipelineRasterizationStateCreateInfo  rasterizationStateCreateInfo = {};
	rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationStateCreateInfo.rasterizerDiscardEnable = false;

	//VkPolygonMode 几何图元生成片段的方式
	//	VK_POLYGON_MODE_FILL：整个多边形，包括多边形内部都产生片段
	//	VK_POLYGON_MODE_LINE：只有多边形的边会产生片段
	//	VK_POLYGON_MODE_POINT：只有多边形的顶点会产生片段
	rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationStateCreateInfo.lineWidth = 1.0f;

	rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT; //剔除背面
	rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; //顺时针的顶点序是正面

	//深度相关设置
	rasterizationStateCreateInfo.depthClampEnable = false;
	rasterizationStateCreateInfo.depthBiasEnable = false;
	rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f; // Optional
	rasterizationStateCreateInfo.depthBiasClamp = 0.0f; // Optional
	rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f; // Optional

	//多重采样
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
	multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleStateCreateInfo.sampleShadingEnable = false;
	multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	//深度测试、模板测试
	//VkPipelineDepthStencilStateCreateInfo 

	//颜色混合：片段着色器返回的片段颜色需要和原来帧缓冲中对应像素的颜色进行混合
	//	混合旧值和新值产生最终的颜色
	//	使用位运算组合旧值和新值
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
	colorBlendAttachmentState.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachmentState.blendEnable = false;

	VkPipelineColorBlendStateCreateInfo colorBlendingStateCreateInfo = {};
	colorBlendingStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendingStateCreateInfo.logicOpEnable = false;
	colorBlendingStateCreateInfo.attachmentCount = 1;
	colorBlendingStateCreateInfo.pAttachments = &colorBlendAttachmentState;

	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
	pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
	//pipelineCreateInfo.pDepthStencilState = 
	pipelineCreateInfo.pColorBlendState = &colorBlendingStateCreateInfo;

	//管线布局
	auto& graphicsPipelineLayout = globalInfoComp->graphicsPipelineLayout;
	pipelineCreateInfo.layout = graphicsPipelineLayout;

	//渲染流程
	auto& renderPass = globalInfoComp->renderPass;
	pipelineCreateInfo.renderPass = renderPass;
	pipelineCreateInfo.subpass = 0; //index

	auto ret = vkCreateGraphicsPipelines(logicDevice, nullptr, 1, &pipelineCreateInfo, nullptr, &globalInfoComp->graphicsPipeline);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create graphicsPipeline error!");
	}
}

void RenderSystem::DestroyGraphicsPipeline(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& graphicsPipeline = globalInfoComp->graphicsPipeline;

	vkDestroyPipeline(logicDevice, graphicsPipeline, nullptr);
}



//顶点输入：描述传递给顶点着色器的顶点数据格式
//	绑定：数据之间的间距和数据是按逐顶点的方式还是按逐实例的方式进行组织
//	属性描述：传递给顶点着色器的属性类型，用于将属性绑定到顶点着色器中的变量
VkPipelineVertexInputStateCreateInfo RenderSystem::GetVertexInputCreateInfo() {

	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
	vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;

	return vertexInputStateCreateInfo;
}