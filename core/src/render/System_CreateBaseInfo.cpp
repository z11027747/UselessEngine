#include <vulkan/vulkan.h>
#include "render/comp.h"
#include "render/system.h"
#include "context.h"

namespace Render {


	void System::CreateBaseInfo(void* ctx) {

		auto* context = (Context*)ctx;
		auto& renderEO = context->renderEO;

		BaseInfo baseInfo;
		baseInfo.type = Type::eBaseInfo;

		//vkInstance
		//	a)负责收集每个应用程序的状态信息
		//	b)创建几乎可执行所有操作的逻辑设备

		//创建之前需要启用哪些扩展（instance级别），比如swapchain负责显示图像
		//Vulkan必须以明确的操作启用，OpenGL则不需要

		//查询有什么扩展
		uint32_t extensionCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions;
		extensions.resize(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		//创建 vkInstance
		std::vector<char const*> desiredExtensions;


		baseInfo.instance = nullptr;

		renderEO.AddComp(baseInfo);

	}



}