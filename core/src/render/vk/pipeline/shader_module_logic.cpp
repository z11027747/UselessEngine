
#include <array>
#include "common/log_system.h"
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pipeline/shader_test_logic.h"
#include "render/vk/pipeline/shader_skybox_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/camera/camera_comp.h"
#include "common/res_system.h"
#include "context.h"
#include "engine_object.h"

namespace Render {

	void ShaderLogic::CreateModules(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto& name = graphicsPipeline->name;

		std::array<VkShaderStageFlagBits, 2> stages = {
			VK_SHADER_STAGE_VERTEX_BIT,
			VK_SHADER_STAGE_FRAGMENT_BIT
		};

		std::array<std::vector<char>, 2> codes = {
			Common::ResSystem::ReadFile("resource/spv/" + name + ".vert.spv"),
			Common::ResSystem::ReadFile("resource/spv/" + name + ".frag.spv")
		};

		for (auto i = 0; i < 2; i++) {
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = codes[i].size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(codes[i].data());

			VkShaderModule shaderModule;
			auto vertRet = vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
			CheckRet(vertRet, "vkCreateShaderModule");

			graphicsPipeline->shaderModules.push_back(shaderModule);

			VkPipelineShaderStageCreateInfo stageCreateInfo = {};
			stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageCreateInfo.stage = stages[i];
			stageCreateInfo.module = shaderModule;
			stageCreateInfo.pName = "main";

			graphicsPipeline->shaderStageCreateInfo.push_back(stageCreateInfo);
		}
	}

	void ShaderLogic::DestroyModules(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto& shaderModules = graphicsPipeline->shaderModules;
		for (auto& shaderModule : shaderModules) {
			vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
		}
	}
}