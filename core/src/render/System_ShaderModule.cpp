
#include "render/comp.h"
#include "context.h"

void RenderSystem::CreateShader(Context* context, const std::string& fileName) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	auto& shaderModules = globalInfoComp->shaderModules;

	std::vector<std::vector<char>> codes = {
		ToolSystem::ReadFile("resource/spv/" + fileName + ".vert.spv"),
		ToolSystem::ReadFile("resource/spv/" + fileName + ".frag.spv")
	};

	for (auto i = 0; i < 2; i++) {
		VkShaderModuleCreateInfo moduleCreateInfo = {};
		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		auto& code = codes[i];
		moduleCreateInfo.codeSize = code.size();
		moduleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		auto vertRet = vkCreateShaderModule(logicDevice, &moduleCreateInfo, nullptr, &shaderModule);
		if (vertRet != VK_SUCCESS) {
			throw std::runtime_error("create vertshaderModule error");
		}

		shaderModules.push_back(std::move(shaderModule));
	}
}

void RenderSystem::DestroyAllShaders(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	auto& shaderModules = globalInfoComp->shaderModules;
	for (auto& shaderModule : shaderModules) {
		vkDestroyShaderModule(logicDevice, shaderModule, nullptr);
	}
}

void RenderSystem::MakeShaderModuleCreateInfos(
	std::vector<VkShaderModule>& shaderModules, std::vector<VkPipelineShaderStageCreateInfo>& stageCreateInfos)
{
	std::vector<VkShaderStageFlagBits> shaderStages = {
		VK_SHADER_STAGE_VERTEX_BIT,
		VK_SHADER_STAGE_FRAGMENT_BIT
	};

	int size = shaderModules.size();

	for (auto i = 0; i < size; i++) {
		VkPipelineShaderStageCreateInfo stageCreateInfo = {};
		stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stageCreateInfo.stage = shaderStages[i];
		stageCreateInfo.module = shaderModules[i];
		stageCreateInfo.pName = "main";

		stageCreateInfos.push_back(stageCreateInfo);
	}
}
