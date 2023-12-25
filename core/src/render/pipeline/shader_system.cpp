
#include <array>
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/logical_device_system.h"
#include "render/global/physical_device_system.h"
#include "render/global/swapchain_system.h"
#include "render/pipeline/pipeline_comp.h"
#include "render/pipeline/pipeline_system.h"
#include "render/pipeline/shader_system.h"
#include "common/res_system.h"
#include "context.h"
#include "base.h"

namespace Render {

	void ShaderSystem::Create(Context* context,
		const std::string& name,
		std::shared_ptr<Pipeline> pipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		std::array<VkShaderStageFlagBits, 2> stages = {
			VK_SHADER_STAGE_VERTEX_BIT,
			VK_SHADER_STAGE_FRAGMENT_BIT
		};

		std::array<std::vector<char>, 2> codes = {
			Common::ResSystem::ReadFile("resource/spv/" + name + ".vert.spv"),
			Common::ResSystem::ReadFile("resource/spv/" + name + ".frag.spv")
		};

		auto shader = std::make_shared<Shader>();
		pipeline->shader = shader;

		for (auto i = 0; i < 2; i++) {
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = codes[i].size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(codes[i].data());

			VkShaderModule shaderModule;
			auto vertRet = vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
			CheckRet(vertRet, "vkCreateShaderModule-Vert");

			VkPipelineShaderStageCreateInfo stageCreateInfo = {};
			stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageCreateInfo.stage = stages[i];
			stageCreateInfo.module = shaderModule;
			stageCreateInfo.pName = "main";

			shader->modules.push_back(shaderModule);
			shader->stageCreateInfo.push_back(stageCreateInfo);
		}
	}

	void ShaderSystem::Destroy(Context* context,
		std::shared_ptr<Pipeline> pipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto& shaderModules = pipeline->shader->modules;
		for (auto& shaderModule : shaderModules) {
			vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
		}
	}

}