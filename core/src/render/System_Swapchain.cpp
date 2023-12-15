#include <GLFW/glfw3.h>
#include "render/comp.h"
#include "render/system.h"
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
	return support;
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

	throw std::runtime_error("find surfaceFormat error");
}


//交换链的兼容表面窗口，需要查询基础属性
//	1.基础表面特性(交换链的最小/最大图像数量，最小/最大图像宽度、高度)
//	2.表面格式(像素格式，颜色空间)
//	3.可用的呈现模式

void RenderSystem::CreateSwapchian(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& physicalDevice = globalInfo.physicalDevice;
	auto& surface = globalInfo.surface;

	//基础表面特性
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface,
		&globalInfo.surfaceCapabilities);

	//表面格式
	uint32_t surfaceFormatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr);

	auto& surfaceFormats = globalInfo.surfaceFormats;
	surfaceFormats.resize(surfaceFormatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());

	//可用的呈现模式
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

	auto& presentModes = globalInfo.presentModes;
	presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

	//需要交换链至少支持一种图像格式 和 一种支持窗口表面的呈现模式



}