#pragma once

#include "vulkan/vulkan.h"
#include "base.h"

enum RenderType
{
	eGlobalInfo = 10001,
};

class RenderGlobal : public EngineComp {
public:

	VkInstance instance;

	bool enableValidationLayer;
	VkDebugUtilsMessengerEXT debugCallback;

	VkPhysicalDevice physicalDevice;
	int physicalDeviceGraphicsFamily;

};