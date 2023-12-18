
#include "render/comp.h"
#include "context.h"


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,		//消息级别
	VkDebugUtilsMessageTypeFlagsEXT messageType,				//消息类型
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,	//回调数据
	void* pUserData												//userData
)
{
	//pCallbackData 回调数据
	//	pMessage：一个以null结尾的包含调试信息的字符串
	//	pObjects：存储有和消息相关的Vulkan对象句柄的数组
	//	objectCount：数组中的对象个数
	std::cerr << "[Custom debugCallback] ===> " << pCallbackData->pMessage << "\n" << std::endl;

	return VK_FALSE;
}

void RenderSystem::GetDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo) {

	debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugCreateInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT		//诊断信息
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT	//警告信息
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;	//不合法和可能造成崩溃的操作信息
	debugCreateInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT			//发生了一些与规范和性能无关的事件
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT	//出现了违反规范的情况或发生了一个可能的错误
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;	//Vulkan
	debugCreateInfo.pfnUserCallback = debugCallback;
	debugCreateInfo.pUserData = nullptr;
}

void RenderSystem::CreateDebugCallback(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& instance = globalInfoComp->instance;

	if (!globalInfoComp->enabledDebug) {
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
	GetDebugCreateInfo(debugCreateInfo);

	//EXT函数不会自动加载
	//vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &callback);

	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func == nullptr) {
		throw std::runtime_error("func error");
	}

	auto ret = func(instance, &debugCreateInfo, nullptr, &globalInfoComp->debugCallback);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("callback error");
	}

}

void RenderSystem::DestroyDebugCallback(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& instance = globalInfoComp->instance;

	if (!globalInfoComp->enabledDebug) {
		return;
	}
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func == nullptr) {
		throw std::runtime_error("func error");
	}

	func(instance, globalInfoComp->debugCallback, nullptr);
}
