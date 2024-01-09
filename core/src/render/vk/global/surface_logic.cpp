
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "context.h"

namespace Render
{

	void SurfaceLogic::Create(Context *context)
	{
		auto &window = context->window;
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &instance = global->instance;

		VkSurfaceKHR surface;
		auto ret = glfwCreateWindowSurface(instance, window, nullptr, &surface);
		CheckRet(ret, "glfwCreateWindowSurface");

		global->surface = surface;
	}

	void SurfaceLogic::Destroy(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &instance = global->instance;
		auto &surface = global->surface;

		vkDestroySurfaceKHR(instance, surface, nullptr);
	}

	bool SurfaceLogic::CheckSupport(Context *context,
									const VkPhysicalDevice &physicalDevice,
									uint32_t queueFamilyIndex)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &surface = global->surface;

		VkBool32 support;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, &support);
		return support == VK_TRUE;
	}

}