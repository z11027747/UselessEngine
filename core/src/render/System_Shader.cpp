
#include "render/comp.h"
#include "context.h"


void RenderSystem::CreateShader(Context* context, const std::string& fileName) {
	auto& renderEO = context->renderEO;

	auto globalInfo = renderEO->GetComponent<RenderGlobal>();
	auto& logicDevice = globalInfo->logicDevice;
	auto& swapchainImageViews = globalInfo->swapchainImageViews;

	auto shaderEO = std::make_shared<EngineObject>();
	auto renderShader = std::make_shared<RenderShader>();

	VkShaderModuleCreateInfo vertCreateInfo = {};
	vertCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	auto vertCode = ToolSystem::ReadFile("resource/spv/" + fileName + ".vert.spv");
	vertCreateInfo.codeSize = vertCode.size();
	vertCreateInfo.pCode = reinterpret_cast<const uint32_t*>(vertCode.data());

	auto vertRet = vkCreateShaderModule(logicDevice, &vertCreateInfo, nullptr, &renderShader->vertModule);
	if (vertRet != VK_SUCCESS) {
		throw std::runtime_error("create vertshaderModule error");
	}

	VkShaderModuleCreateInfo fragCreateInfo = {};
	fragCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	auto fragCode = ToolSystem::ReadFile("resource/spv/" + fileName + ".frag.spv");
	fragCreateInfo.codeSize = fragCode.size();
	fragCreateInfo.pCode = reinterpret_cast<const uint32_t*>(fragCode.data());

	auto fragRet = vkCreateShaderModule(logicDevice, &fragCreateInfo, nullptr, &renderShader->fragModule);
	if (fragRet != VK_SUCCESS) {
		throw std::runtime_error("create fragshaderModule error");
	}

	shaderEO->AddComponent<RenderShader>(renderShader);

	context->shaderEOMap.emplace(fileName, shaderEO);
}


void RenderSystem::DestroyAllShaders(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfo = renderEO->GetComponent<RenderGlobal>();
	auto& logicDevice = globalInfo->logicDevice;

	auto& shaderEOMap = context->shaderEOMap;
	for (const auto& [key, value] : shaderEOMap) {

		auto renderShader = value->GetComponent<RenderShader>();
		vkDestroyShaderModule(logicDevice, renderShader->vertModule, nullptr);
		vkDestroyShaderModule(logicDevice, renderShader->fragModule, nullptr);
	}
}