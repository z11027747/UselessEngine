
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/instance_logic.h"
#include "context.h"

namespace Render
{
	void InstanceLogic::Create(Context *context,
							   std::vector<const char *> windowExtensions,
							   bool enabledDebug)
	{
		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; // 最后会转成void*存，需要定义类型

		VkApplicationInfo applicationInfo = {};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = "UselessEngine";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
		auto apiVersion = GetApiVersion();
		applicationInfo.apiVersion = apiVersion;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;

		std::vector<const char *> enabledExtensions(windowExtensions);
		if (enabledDebug)
		{
			enabledExtensions.push_back("VK_EXT_debug_utils");
		}
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();

		std::vector<const char *> enabledLayers;
		VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};

		if (enabledDebug)
		{
			enabledLayers.push_back("VK_LAYER_KHRONOS_validation");

			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = enabledLayers.data();

			// MakeDebugUtilsMessengerCreateInfo(debugUtilsMessengerCreateInfo);
			// instanceCreateInfo.pNext = reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugUtilsMessengerCreateInfo);
		}
		else
		{
			instanceCreateInfo.enabledLayerCount = 0;
			instanceCreateInfo.pNext = nullptr;
		}

		VkInstance vkInstance;
		auto ret = vkCreateInstance(&instanceCreateInfo, nullptr, &vkInstance);
		CheckRet(ret, "vkCreateInstance");

		auto global = std::make_shared<Global>();
		global->instance = vkInstance;

		auto &renderGlobalEO = context->renderGlobalEO;
		renderGlobalEO->AddComponent<Global>(global);
	}

	void InstanceLogic::Destroy(Context *context)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto &global = renderGlobalEO->GetComponent<Global>();
		vkDestroyInstance(global->instance, nullptr);
	}

	void InstanceLogic::CreateDebugCallback(Context *context)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto &global = renderGlobalEO->GetComponent<Global>();
		auto &instance = global->instance;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		MakeDebugUtilsMessengerCreateInfo(createInfo);

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func == nullptr)
		{
			throw std::runtime_error("get vkCreateDebugUtilsMessengerEXT error");
		}

		VkDebugUtilsMessengerEXT vkDebugUtilsMessenger;
		auto ret = func(instance, &createInfo, nullptr, &vkDebugUtilsMessenger);
		CheckRet(ret, "vkCreateDebugUtilsMessengerEXT");

		global->debugUtilsMessenger = vkDebugUtilsMessenger;
	}

	void InstanceLogic::DestroyDebugCallback(Context *context)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto &global = renderGlobalEO->GetComponent<Global>();
		auto &instance = global->instance;
		auto &debugUtilsMessenger = global->debugUtilsMessenger;

		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func == nullptr)
		{
			throw std::runtime_error("get vkDestroyDebugUtilsMessengerEXT error");
		}

		func(instance, debugUtilsMessenger, nullptr);
	}

	uint32_t InstanceLogic::GetApiVersion()
	{
		uint32_t apiVersion;
		auto ret = vkEnumerateInstanceVersion(&apiVersion);
		CheckRet(ret, "vkEnumerateInstanceVersion");

		uint32_t major = VK_VERSION_MAJOR(apiVersion);
		uint32_t minor = VK_VERSION_MINOR(apiVersion);
		uint32_t patch = VK_VERSION_PATCH(apiVersion);

		Common::LogSystem::Info("VkInstance ApiVersion: " + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch));

		return apiVersion;
	}

	bool InstanceLogic::CheckExtension(const char *extensionName)
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions;

		extensions.resize(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		for (auto &extension : extensions)
		{
			std::cout << "name: " << extension.extensionName << "\n";
			if (strcmp(extension.extensionName, extensionName) == 0)
			{
				return true;
			}
		}

		return false;
	}

	bool InstanceLogic::CheckLayer(const char *layerName)
	{
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> layers;
		layers.resize(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		for (auto &layer : layers)
		{
			std::cout << "name: " << layer.layerName << "\n";
			if (strcmp(layer.layerName, layerName) == 0)
			{
				return true;
			}
		}

		return false;
	}

}