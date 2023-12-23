
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/device/physical_device_system.h"
#include "render/global/surface/surface_system.h"
#include "context.h"

namespace Render {

	void SurfaceSystem::Create(Context* context) {
		auto& window = context->window;
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& instance = global->instance;

		VkSurfaceKHR surface;
		auto ret = glfwCreateWindowSurface(instance, window, nullptr, &surface);
		CheckRet(ret, "glfwCreateWindowSurface");

		global->surface = surface;
	}

	void SurfaceSystem::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& instance = global->instance;
		auto& surface = global->surface;

		vkDestroySurfaceKHR(instance, surface, nullptr);
	}

	bool SurfaceSystem::CheckSupport(Context* context,
		const VkPhysicalDevice& physicalDevice,
		uint32_t queueFamilyIndex
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& surface = global->surface;

		VkBool32 support;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, &support);
		return support == VK_TRUE;
	}

}
