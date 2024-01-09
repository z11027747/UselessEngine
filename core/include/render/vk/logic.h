#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "common/log_system.h"

namespace Render
{
    inline static std::vector<const char *> GetWindowExtensions()
    {
        uint32_t glfwExtensionCount;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char *> windowExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        return windowExtensions;
    }

    static std::string ToString(VkResult errorCode)
    {
        switch (errorCode)
        {
#define STR(r)   \
    case VK_##r: \
        return #r
            STR(NOT_READY);
            STR(TIMEOUT);
            STR(EVENT_SET);
            STR(EVENT_RESET);
            STR(INCOMPLETE);
            STR(ERROR_OUT_OF_HOST_MEMORY);
            STR(ERROR_OUT_OF_DEVICE_MEMORY);
            STR(ERROR_INITIALIZATION_FAILED);
            STR(ERROR_DEVICE_LOST);
            STR(ERROR_MEMORY_MAP_FAILED);
            STR(ERROR_LAYER_NOT_PRESENT);
            STR(ERROR_EXTENSION_NOT_PRESENT);
            STR(ERROR_FEATURE_NOT_PRESENT);
            STR(ERROR_INCOMPATIBLE_DRIVER);
            STR(ERROR_TOO_MANY_OBJECTS);
            STR(ERROR_FORMAT_NOT_SUPPORTED);
            STR(ERROR_SURFACE_LOST_KHR);
            STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
            STR(SUBOPTIMAL_KHR);
            STR(ERROR_OUT_OF_DATE_KHR);
            STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
            STR(ERROR_VALIDATION_FAILED_EXT);
            STR(ERROR_INVALID_SHADER_NV);
            STR(ERROR_INCOMPATIBLE_SHADER_BINARY_EXT);
#undef STR
        default:
            return "UNKNOWN_ERROR";
        }
    }

    inline static void CheckRet(VkResult ret)
    {
        if (ret != VK_SUCCESS)
        {
            Common::LogSystem::Exception(std::string("imgui: ") + ToString(ret));
        }
    }

    inline static void CheckRet(VkResult ret, const std::string &info)
    {
        if (ret != VK_SUCCESS)
        {
            Common::LogSystem::Exception(info + std::string(": ") + ToString(ret));
        }
    }

    static VkBool32 userCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            Common::LogSystem::Exception("Vk userCallback: " + std::string(pCallbackData->pMessage));
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
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT  // 警告信息
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; // 不合法和可能造成崩溃的操作信息
        debugUtilsMessengerCreateInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT        // 发生了一些与规范和性能无关的事件
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT   // 出现了违反规范的情况或发生了一个可能的错误
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT; // Vulkan
        debugUtilsMessengerCreateInfo.pfnUserCallback = userCallback;
        debugUtilsMessengerCreateInfo.pUserData = nullptr;
    }
}