#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "render/comp.h"
#include "render/system.h"
#include "context.h"

void RenderSystem::CreateVKInstance(Context* context) {

	auto& renderEO = context->renderEO;
	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();

	uint32_t apiVersion;
	auto versionRet = vkEnumerateInstanceVersion(&apiVersion);
	if (versionRet != VK_SUCCESS) {
		apiVersion = VK_API_VERSION_1_3;
	}

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; //最后会转成void*存，需要定义类型
	applicationInfo.pApplicationName = "UselessEngine";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pEngineName = "No Engine";
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.apiVersion = apiVersion;

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	//Vulkan不关心平台，用glfw获取和窗口系统交互的扩展
	uint32_t glfwExtensionCount;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> enabledExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (globalInfo.enableValidationLayer) {
		enabledExtensions.push_back("VK_EXT_debug_utils");
	}

	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();

	std::vector<const char*> enabledLayers;
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};

	if (globalInfo.enableValidationLayer) {
		enabledLayers.push_back("VK_LAYER_KHRONOS_validation");

		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = enabledLayers.data();

		//创建实例的同事，扩展debugcallback，把配置传过去
		CreateDebugCreateInfo(debugCreateInfo);
		instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.pNext = nullptr;
	}

	auto ret = vkCreateInstance(&instanceCreateInfo, nullptr, &globalInfo.instance);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create instance error");
	}
}

void RenderSystem::DestroyVKInstance(Context* context) {

	auto& renderEO = context->renderEO;
	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();

	vkDestroyInstance(globalInfo.instance, nullptr);
	globalInfo.instance = nullptr;
}

bool RenderSystem::CheckInstanceExtension(const char* extensionName) {
	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions;

	extensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	for (auto& extension : extensions) {
		std::cout << "name: " << extension.extensionName << "\n";
		if (strcmp(extension.extensionName, extensionName) == 0) {
			return true;
		}
	}

	return false;
}

bool RenderSystem::CheckInstanceLayer(const char* layerName) {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> layers;
	layers.resize(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

	for (auto& layer : layers) {
		std::cout << "name: " << layer.layerName << "\n";
		if (strcmp(layer.layerName, layerName) == 0) {
			return true;
		}
	}

	return false;
}