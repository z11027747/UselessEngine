
#include "render/comp.h"
#include "context.h"

//管线布局
//	在着色器中使用uniform变量，实现对着色器进行一定程度的动态配置
//	在着色器中使用的uniform变量需要在管线创建时使用VkPipelineLayout对象定义
void RenderSystem::CreatePipelineLayout(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& pipelineLayout = globalInfoComp->graphicsPipelineLayout;

	VkPipelineLayoutCreateInfo layoutCreateInfo = {};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	//描述符
	auto& descriptorSetLayout = globalInfoComp->descriptorSetLayout;
	layoutCreateInfo.setLayoutCount = 1;
	layoutCreateInfo.pSetLayouts = &descriptorSetLayout;

	auto ret = vkCreatePipelineLayout(logicDevice, &layoutCreateInfo, nullptr, &pipelineLayout);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create pipelineLayout error!");
	}
}

void RenderSystem::DestroyPipelineLayout(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& pipelineLayout = globalInfoComp->graphicsPipelineLayout;

	vkDestroyPipelineLayout(logicDevice, pipelineLayout, nullptr);
}