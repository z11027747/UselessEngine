#pragma once

#include <vector>

class Context;
class VkDebugUtilsMessengerCreateInfoEXT;

class RenderSystem final {
public:

	static void CreateGlobal(Context*);

	//instance
	static void CreateVKInstance(Context*);
	static void DestroyVKInstance(Context*);
	static bool CheckInstanceExtension(const char*);
	static bool CheckInstanceLayer(const char*);

	//debugCallback
	static void CreateDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);
	static void CreateDebugCallback(Context*);

	//physicsDevice
	static void PickupPhysicalDevice(Context*);

	//logicDevice
	static void CreateLogicDevice(Context*);


};