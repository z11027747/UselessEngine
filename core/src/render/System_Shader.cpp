
#include "render/comp.h"
#include "context.h"


void RenderSystem::CreateShader(Context* context, const std::string& fileName) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapchainImageViews = globalInfoComp->swapchainImageViews;

	auto renderShader = std::make_shared<RenderShader>();

	auto& modules = renderShader->modules;
	auto& stageCreateInfos = renderShader->stageCreateInfos;

	std::vector<std::vector<char>> codes = {
		ToolSystem::ReadFile("resource/spv/" + fileName + ".vert.spv"),
		ToolSystem::ReadFile("resource/spv/" + fileName + ".frag.spv")
	};
	std::vector< VkShaderStageFlagBits> flagBits = {
		VK_SHADER_STAGE_VERTEX_BIT,
		VK_SHADER_STAGE_FRAGMENT_BIT
	};

	modules.resize(2);
	stageCreateInfos.resize(2);

	for (auto i = 0; i < 2; i++) {
		VkShaderModuleCreateInfo moduleCreateInfo = {};
		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		auto& code = codes[i];
		moduleCreateInfo.codeSize = code.size();
		moduleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		auto vertRet = vkCreateShaderModule(logicDevice, &moduleCreateInfo, nullptr, &modules[i]);
		if (vertRet != VK_SUCCESS) {
			throw std::runtime_error("create vertshaderModule error");
		}

		auto& stageCreateInfo = stageCreateInfos[i];
		stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stageCreateInfo.stage = flagBits[i];
		stageCreateInfo.module = modules[i];
		stageCreateInfo.pName = "main";
	}

	globalInfoComp->shaderMap.emplace(fileName, renderShader);
}

void RenderSystem::DestroyAllShaders(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	auto& shaderEOMap = globalInfoComp->shaderMap;
	for (const auto& [key, renderShader] : shaderEOMap) {
		for (auto& module : renderShader->modules) {
			vkDestroyShaderModule(logicDevice, module, nullptr);
		}
	}
}

