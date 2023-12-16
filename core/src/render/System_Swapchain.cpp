
#include "render/comp.h"
#include "context.h"

//支持呈现图像到窗口表面能力的队列族
bool RenderSystem::CheckSwapchainSupport(Context* context,
	const VkPhysicalDevice& physicalDevice, int i)
{
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& surface = globalInfo.surface;

	VkBool32 support;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &support);
	return support == VK_TRUE;
}

//VkSurfaceFormatKHR 表面格式
//	format     指定颜色通道和存储类型
//	colorspace 表示SRGB颜色空间是否被支持
//		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR  
VkSurfaceFormatKHR RenderSystem::GetSwapchainSurfaceFormat(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& physicalDevice = globalInfo.physicalDevice;
	auto& surface = globalInfo.surface;

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
//			这一模式可以用来实现三倍缓冲，避免撕裂现象的同时减小了延迟问题。
VkPresentModeKHR RenderSystem::GetSwapchainPresentMode(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& physicalDevice = globalInfo.physicalDevice;
	auto& surface = globalInfo.surface;

	//可用的呈现模式
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
VkSurfaceCapabilitiesKHR RenderSystem::GetSwapchainCapbilities(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& physicalDevice = globalInfo.physicalDevice;
	auto& surface = globalInfo.surface;

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	return surfaceCapabilities;
}

void RenderSystem::CreateSwapchian(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& logicDevice = globalInfo.logicDevice;
	auto& surface = globalInfo.surface;

	auto surfaceFormat = GetSwapchainSurfaceFormat(context);
	auto presentMode = GetSwapchainPresentMode(context);
	auto surfaceCapabilities = GetSwapchainCapbilities(context);

	//需要交换链至少支持一种图像格式 和 一种支持窗口表面的呈现模式

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount + 1; //3缓冲
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
	swapchainCreateInfo.presentMode = presentMode;

	//clipped为VK_TRUE表示我们不关心被窗口系统中的其它窗口遮挡的像素的颜色
	//	但如果我们回读窗口的像素值就可能出现问题。
	swapchainCreateInfo.clipped = true;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	auto ret = vkCreateSwapchainKHR(logicDevice, &swapchainCreateInfo, nullptr, &globalInfo.swapchain);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create swapchain error");
	}

	globalInfo.swapChainImageFormat = surfaceFormat.format;
	globalInfo.swapChainExtent = surfaceCapabilities.currentExtent;
}

void RenderSystem::DestroySwapchian(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& logicDevice = globalInfo.logicDevice;
	auto& swapchain = globalInfo.swapchain;

	vkDestroySwapchainKHR(logicDevice, swapchain, nullptr);
}

//获取交换链图像的图像句柄，之后使用这些图像句柄进行渲染操作
void RenderSystem::GetSwapchianImages(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& logicDevice = globalInfo.logicDevice;
	auto& swapchain = globalInfo.swapchain;

	uint32_t swapchainImageCount;
	vkGetSwapchainImagesKHR(logicDevice, swapchain, &swapchainImageCount, nullptr);

	auto& swapchainImages = globalInfo.swapchainImages;
	swapchainImages.resize(swapchainImageCount);
	vkGetSwapchainImagesKHR(logicDevice, swapchain, &swapchainImageCount, swapchainImages.data());
}

//任何VkImage对象，都需要创建一个VkImageView对象来绑定访问它
void RenderSystem::CreateSwapchianImageViews(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& logicDevice = globalInfo.logicDevice;
	auto& swapchain = globalInfo.swapchain;
	auto swapChainImageFormat = globalInfo.swapChainImageFormat;
	auto& swapChainExtent = globalInfo.swapChainExtent;
	auto& swapchainImages = globalInfo.swapchainImages;

	auto swapchainImageCount = swapchainImages.size();

	auto& swapchainImageViews = globalInfo.swapchainImageViews;
	swapchainImageViews.resize(swapchainImageCount);

	for (uint32_t i = 0; i < swapchainImageCount; i++) {

		auto& swapchainImage = swapchainImages[i];

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = swapchainImage;
		imageViewCreateInfo.format = swapChainImageFormat;
		imageViewCreateInfo.viewType = VkImageViewType::VK_IMAGE_VIEW_TYPE_2D; //二维纹理

		//进行图像颜色通道的映射
		VkComponentMapping mapping = {};
		mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		mapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		mapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		mapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components = mapping;

		//指定图像的用途和图像的哪一部分可以被访问
		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange = subresourceRange;

		auto ret = vkCreateImageView(logicDevice, &imageViewCreateInfo, nullptr, &swapchainImageViews[i]);
		if (ret != VK_SUCCESS) {
			throw std::runtime_error("create imageView error");
		}
	}
}

void RenderSystem::DestroySwapchianImageViews(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& logicDevice = globalInfo.logicDevice;
	auto& swapchainImageViews = globalInfo.swapchainImageViews;

	for (auto& swapchainImageView : swapchainImageViews) {
		vkDestroyImageView(logicDevice, swapchainImageView, nullptr);
	}
}