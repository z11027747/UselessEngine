﻿
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/global/surface_logic.h"
#include "context.h"

namespace Render {

	void SurfaceLogic::Create(Context* context) {
		auto& window = context->window;
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& instance = global->instance;

		VkSurfaceKHR surface;
		auto ret = glfwCreateWindowSurface(instance, window, nullptr, &surface);
		CheckRet(ret, "glfwCreateWindowSurface");

		global->surface = surface;
	}

	void SurfaceLogic::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& instance = global->instance;
		auto& surface = global->surface;

		vkDestroySurfaceKHR(instance, surface, nullptr);
	}

	bool SurfaceLogic::CheckSupport(Context* context,
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