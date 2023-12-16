
#include "render/comp.h"
#include "context.h"

//VkSurfaceKHR对象是平台无关的，是个扩展，指的就是窗口
// 但它的创建依赖窗口系统，glfw
void RenderSystem::CreateSurface(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& instance = globalInfo.instance;

	//glfwCreateWindowSurface 本质上就做了这件事
	// VkWin32SurfaceCreateInfoKHR createInfo = {};
	// createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	// createInfo.hwnd = glfwGetWin32Window(window);
	// createInfo.hinstance = GetModuleHandle(nullptr);

	auto ret = glfwCreateWindowSurface(instance, context->window, nullptr, &globalInfo.surface);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create surface error");
	}
}

void RenderSystem::DestroySurface(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& instance = globalInfo.instance;
	auto& surfaceKHR = globalInfo.surface;

	vkDestroySurfaceKHR(instance, surfaceKHR, nullptr);
}