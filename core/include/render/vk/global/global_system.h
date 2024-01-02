#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "common/log_system.h"

namespace Render
{

	inline static void CheckRet(VkResult ret, const std::string &info)
	{
		if (ret != VK_SUCCESS)
		{
			Common::LogSystem::Error(info);
			throw std::runtime_error(info);
		}
	}

	inline static std::vector<const char *> GetWindowExtensions()
	{
		uint32_t glfwExtensionCount;
		const char **glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char *> windowExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		return windowExtensions;
	}

	static VkBool32 userCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
		void *pUserData)
	{
		// pCallbackData 回调数据
		//	pMessage：一个以null结尾的包含调试信息的字符串
		//	pObjects：存储有和消息相关的Vulkan对象句柄的数组
		//	objectCount：数组中的对象个数
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			Common::LogSystem::Error("Vk userCallback: " + std::string(pCallbackData->pMessage));
		}
		else
		{
			Common::LogSystem::Debug("Vk userCallback: " + std::string(pCallbackData->pMessage));
		}

		return VK_FALSE;
	}

	inline static void MakeDebugUtilsMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &debugUtilsMessengerCreateInfo)
	{
		debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugUtilsMessengerCreateInfo.messageSeverity =
			// VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT		//诊断信息
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT	 // 警告信息
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; // 不合法和可能造成崩溃的操作信息
		debugUtilsMessengerCreateInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT		   // 发生了一些与规范和性能无关的事件
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT   // 出现了违反规范的情况或发生了一个可能的错误
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT; // Vulkan
		debugUtilsMessengerCreateInfo.pfnUserCallback = userCallback;
		debugUtilsMessengerCreateInfo.pUserData = nullptr;
	}

}
