
#include "render/comp.h"
#include "context.h"

//支持呈现图像到窗口表面能力的队列族
bool RenderSystem::CheckSwapchainSupport(Context* context,
	const VkPhysicalDevice& physicalDevice, int index)
{
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& surface = globalInfoComp->surface;

	VkBool32 support;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &support);
	return support == VK_TRUE;
}

void RenderSystem::CreateSwapchian(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& surface = globalInfoComp->surface;

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

	auto ret = vkCreateSwapchainKHR(logicDevice, &swapchainCreateInfo, nullptr, &globalInfoComp->swapchain);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create swapchain error");
	}

	globalInfoComp->swapChainImageFormat = surfaceFormat.format;
	globalInfoComp->swapChainExtent = surfaceCapabilities.currentExtent;
}


void RenderSystem::DestroySwapchian(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapchain = globalInfoComp->swapchain;

	vkDestroySwapchainKHR(logicDevice, swapchain, nullptr);
}

//VkSurfaceFormatKHR 表面格式
//	format     指定颜色通道和存储类型
//	colorspace 表示SRGB颜色空间是否被支持
//		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR  
VkSurfaceFormatKHR RenderSystem::GetSwapchainSurfaceFormat(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& physicalDevice = globalInfoComp->physicalDevice;
	auto& surface = globalInfoComp->surface;

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
VkPresentModeKHR RenderSystem::GetSwapchainPresentMode(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& physicalDevice = globalInfoComp->physicalDevice;
	auto& surface = globalInfoComp->surface;

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

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& physicalDevice = globalInfoComp->physicalDevice;
	auto& surface = globalInfoComp->surface;

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	return surfaceCapabilities;
}

//获取交换链图像的图像句柄，之后使用这些图像句柄进行渲染操作
void RenderSystem::GetSwapchianImages(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapchain = globalInfoComp->swapchain;

	uint32_t swapchainImageCount = 0;
	vkGetSwapchainImagesKHR(logicDevice, swapchain, &swapchainImageCount, nullptr);

	auto& swapchainImages = globalInfoComp->swapchainImages;
	swapchainImages.resize(swapchainImageCount);
	vkGetSwapchainImagesKHR(logicDevice, swapchain, &swapchainImageCount, swapchainImages.data());
}

//任何VkImage对象，都需要创建一个VkImageView对象来绑定访问它
void RenderSystem::CreateSwapchianImageViews(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapchain = globalInfoComp->swapchain;
	auto swapChainImageFormat = globalInfoComp->swapChainImageFormat;
	auto& swapChainExtent = globalInfoComp->swapChainExtent;
	auto& swapchainImages = globalInfoComp->swapchainImages;

	auto swapchainImageCount = swapchainImages.size();

	auto& swapchainImageViews = globalInfoComp->swapchainImageViews;
	swapchainImageViews.resize(swapchainImageCount);

	for (uint32_t i = 0; i < swapchainImageCount; i++) {

		auto& swapchainImage = swapchainImages[i];
		auto& swapchainImageView = swapchainImageViews[i];

		CreateImageView(context,
			swapChainImageFormat, swapchainImage, swapchainImageView);
	}
}

void RenderSystem::DestroySwapchianImageViews(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& swapchainImageViews = globalInfoComp->swapchainImageViews;

	for (auto& swapchainImageView : swapchainImageViews) {
		DestroyImageView(context, swapchainImageView);
	}
}

void RenderSystem::SetNeedRecreateSwapchain(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	if (globalInfoComp->needRecreateSwapchain) {
		return;
	}

	globalInfoComp->needRecreateSwapchain = true;
	WaitIdle(context);
}

//重建交换链
void RenderSystem::TryRecreateSwapchain(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	if (!globalInfoComp->needRecreateSwapchain) {
		return;
	}
	globalInfoComp->needRecreateSwapchain = false;

	//等待设备处于空闲状态，避免在对象的使用过程中将其清除重建
	WaitIdle(context);

	//清空老数据
	FreeSwapchainCommandBuffers(context);
	DestroyFrameBuffers(context);
	DestroyGraphicsPipeline(context);
	DestroyRenderPass(context);
	DestroySwapchianImageViews(context);
	DestroySwapchian(context);

	//重新创建交换链
	CreateSwapchian(context);

	//图形视图是直接依赖于交换链图像的，所以也需要被重建图像视图
	GetSwapchianImages(context);
	CreateSwapchianImageViews(context);

	//渲染流程依赖于交换链图像的格式
	//	虽然像窗口大小改变不会引起使用的交换链图像格式改变，但我们还是应该对它进行处理。
	CreateRenderPass(context);

	//视口和裁剪矩形在管线创建时被指定，窗口大小改变，也需要重建管线
	CreateGraphicsPipeline(context);

	//帧缓冲和指令缓冲直接依赖于交换链图像，也需要重建
	CreateFrameBuffers(context);
	AllocateSwapchainCommandBuffers(context);
}

void RenderSystem::AllocateSwapchainCommandBuffers(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();

	auto& swapchainFrameBuffers = globalInfoComp->swapchainFrameBuffers;
	auto size = swapchainFrameBuffers.size();

	auto& swapchainCommandBuffers = globalInfoComp->swapchainCommandBuffers;
	swapchainCommandBuffers.resize(size);

	AllocateCommandBuffer(context, swapchainCommandBuffers);
}

void RenderSystem::FreeSwapchainCommandBuffers(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();

	auto& swapchainCommandBuffers = globalInfoComp->swapchainCommandBuffers;
	FreeCommandBuffer(context, swapchainCommandBuffers);
}