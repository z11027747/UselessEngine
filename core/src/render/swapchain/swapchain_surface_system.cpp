
#include "render/global/global_def.h"
#include "render/instance/instance_comp.h"
#include "render/device/device_comp.h"
#include "render/device/logical_device_system.h"
#include "render/device/physical_device_system.h"
#include "render/device/physical_device_system.h"
#include "render/swapchain/swapchain_comp.h"
#include "render/swapchain/swapchain_surface_system.h"
#include "context.h"

namespace Render {

	void SwapchainSurfaceSystem::Create(Context* context,
		std::shared_ptr<Swapchain> swapchain
	) {
		auto& window = context->window;
		auto& renderEO = context->renderEO;

		auto instance = renderEO->GetComponent<Instance>();
		auto& vkInstance = instance->vkInstance;

		//glfwCreateWindowSurface 本质上就做了这件事
		//	VkWin32SurfaceCreateInfoKHR createInfo = {};
		//	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		//	createInfo.hwnd = glfwGetWin32Window(window);
		//	createInfo.hinstance = GetModuleHandle(nullptr);

		VkSurfaceKHR surface;
		auto ret = glfwCreateWindowSurface(vkInstance, window, nullptr, &surface);
		CheckRet(ret, "glfwCreateWindowSurface");

		swapchain->surface = surface;
		swapchain->surfaceFormat = SwapchainSurfaceSystem::GetFormat(context);
		swapchain->surfacePresentMode = SwapchainSurfaceSystem::GetPresentMode(context);
		swapchain->surfaceCapabilities = SwapchainSurfaceSystem::GetCapbilities(context);
	}

	void SwapchainSurfaceSystem::Destroy(Context* context,
		std::shared_ptr<Swapchain> swapchain
	) {
		auto& renderEO = context->renderEO;

		auto instance = renderEO->GetComponent<Instance>();
		auto& vkInstance = instance->vkInstance;

		auto& surface = swapchain->surface;
		vkDestroySurfaceKHR(vkInstance, surface, nullptr);
	}

	bool SwapchainSurfaceSystem::CheckSupport(Context* context,
		const VkPhysicalDevice& physicalDevice,
		uint32_t queueFamilyIndex
	) {
		auto& renderEO = context->renderEO;

		auto swapchain = renderEO->GetComponent<Swapchain>();
		auto& surface = swapchain->surface;

		VkBool32 support;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, &support);
		return support == VK_TRUE;
	}

	//VkSurfaceFormatKHR 表面格式
	//	format     指定颜色通道和存储类型
	//	colorspace 表示SRGB颜色空间是否被支持
	//		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR  
	VkSurfaceFormatKHR SwapchainSurfaceSystem::GetFormat(Context* context) {
		auto& renderEO = context->renderEO;

		auto device = renderEO->GetComponent<Device>();
		auto& physicalDevice = device->physicalDevice;

		auto swapchain = renderEO->GetComponent<Swapchain>();
		auto& surface = swapchain->surface;

		//表面格式
		uint32_t surfaceFormatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr);

		std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());

		//选择期望的格式支持
		for (const auto& surfaceFormat : surfaceFormats) {
			if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM
				&& surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return surfaceFormat;
			}
		}

		return surfaceFormats[0];
	}

	//可用的呈现模式
	//	VK_PRESENT_MODE_IMMEDIATE_KHR：应用程序提交的图像会被立即传输到屏幕上，可能会导致撕裂现象。
	//	VK_PRESENT_MODE_FIFO_KHR：交换链变成一个先进先出的队列，每次从队列头部取出一张图像进行显示，应用程序渲染的图像提交给交换链后，会被放在队列尾部。
	//			当队列为满时，应用程序需要进行等待。这一模式非常类似现在常用的垂直同步。刷新显示的时刻也被叫做垂直回扫。
	//	VK_PRESENT_MODE_FIFO_RELAXED_KHR：这一模式和上一模式的唯一区别是，如果应用程序延迟，导致交换链的队列在上一次垂直回扫时为空，
	//			那么，如果应用程序在下一次垂直回扫前提交图像，图像会立即被显示。这一模式可能会导致撕裂现象。
	//	VK_PRESENT_MODE_MAILBOX_KHR：这一模式是第二种模式的另一个变种。它不会在交换链的队列满时阻塞应用程序，队列中的图像会被直接替换为应用程序新提交的图像。
	//			这一模式可以用来 实现三倍缓冲，避免撕裂现象的同时减小了延迟问题。
	VkPresentModeKHR SwapchainSurfaceSystem::GetPresentMode(Context* context) {
		auto& renderEO = context->renderEO;

		auto device = renderEO->GetComponent<Device>();
		auto& physicalDevice = device->physicalDevice;

		auto swapchain = renderEO->GetComponent<Swapchain>();
		auto& surface = swapchain->surface;

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

		for (auto& presentMode : presentModes) {
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return presentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	//基础表面特性(交换链的最小/最大图像数量，最小/最大图像宽度、高度)
	VkSurfaceCapabilitiesKHR SwapchainSurfaceSystem::GetCapbilities(Context* context) {
		auto& renderEO = context->renderEO;

		auto device = renderEO->GetComponent<Device>();
		auto& physicalDevice = device->physicalDevice;

		auto swapchain = renderEO->GetComponent<Swapchain>();
		auto& surface = swapchain->surface;

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

		return surfaceCapabilities;
	}

}
