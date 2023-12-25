
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/logical_device_system.h"
#include "render/global/physical_device_system.h"
#include "render/global/swapchain_system.h"
#include "render/pipeline/pipeline_comp.h"
#include "render/pipeline/pipeline_system.h"
#include "render/pipeline/pipeline_layout_system.h"
#include "common/res_system.h"
#include "context.h"
#include "base.h"

namespace Render {

	void PipelineLayoutSystem::Create(Context* context,
		std::shared_ptr<Pipeline> pipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		VkPipelineLayoutCreateInfo layoutCreateInfo = {};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		auto& descriptorSetLayout = pipeline->descriptorSetLayout;
		layoutCreateInfo.setLayoutCount = 1;
		layoutCreateInfo.pSetLayouts = &descriptorSetLayout;

		VkPipelineLayout pipelineLayout;
		auto ret = vkCreatePipelineLayout(logicalDevice, &layoutCreateInfo, nullptr, &pipelineLayout);
		CheckRet(ret, "vkCreatePipelineLayout");

		pipeline->pipelineLayout = pipelineLayout;
	}

	void PipelineLayoutSystem::Destroy(Context* context,
		std::shared_ptr<Pipeline> pipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto& pipelineLayout = pipeline->pipelineLayout;
		vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	}

}