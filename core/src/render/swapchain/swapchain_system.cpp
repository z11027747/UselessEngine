
#include "render/global/global_def.h"
#include "render/device/device_comp.h"
#include "render/device/logical_device_system.h"
#include "render/device/physical_device_system.h"
#include "render/swapchain/swapchain_comp.h"
#include "render/swapchain/swapchain_system.h"
#include "render/swapchain/swapchain_surface_system.h"
#include "context.h"

namespace Render {

	void SwapchainSystem::Create(Context* context) {

		auto& renderEO = context->renderEO;

		//TODO
	}

	void SwapchainSystem::CreateVKSwapchain(Context* context,
		std::shared_ptr<Swapchain> swapchain
	) {
		auto& renderEO = context->renderEO;

		auto device = renderEO->GetComponent<Device>();
		auto& physicalDevice = device->physicalDevice;
		auto& logicalDevice = device->logicalDevice;

		auto& surface = swapchain->surface;
		auto& surfaceFormat = swapchain->surfaceFormat;
		auto& surfacePresentMode = swapchain->surfacePresentMode;
		auto& surfaceCapabilities = swapchain->surfaceCapabilities;

		//交换链至少支持一种图像格式 和 一种支持窗口表面的呈现模式

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount; //2缓冲
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
		swapchainCreateInfo.imageArrayLayers = 1; //imageArrayLayers成员变量用于指定每个图像所包含的层次 一般是1 除非VR
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //作为传输的目的图像

		//VkSharingMode 描述图像共享模式
		//	VK_SHARING_MODE_EXCLUSIVE：一张图像同一时间只能被一个队列族所拥有
		//		在另一队列族使用它之前，必须显式地改变图像所有权。这一模式下性能表现最佳。
		//	VK_SHARING_MODE_CONCURRENT：图像可以在多个队列族间使用，不需要显式地改变图像所有权。
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform; //图形变换操作，可以反转之类的
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; //忽略alpha通道
		swapchainCreateInfo.presentMode = surfacePresentMode;

		//clipped为VK_TRUE表示我们不关心被窗口系统中的其它窗口遮挡的像素的颜色
		//	但如果我们回读窗口的像素值就可能出现问题。
		swapchainCreateInfo.clipped = true;
		swapchainCreateInfo.oldSwapchain = nullptr;

		VkSwapchainKHR vkSwapchain;
		auto ret = vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &vkSwapchain);
		CheckRet(ret, "vkCreateSwapchainKHR");

		swapchain->vkSwapchain = vkSwapchain;
	}


}
