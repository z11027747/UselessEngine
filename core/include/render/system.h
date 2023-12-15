#pragma once

#include <vector>

class Context;
struct VkDebugUtilsMessengerCreateInfoEXT;

class RenderSystem final {
public:

	static void CreateGlobal(Context*);

	//instance
	static void CreateInstance(Context*);
	static void DestroyInstance(Context*);
	static bool CheckInstanceExtension(const char*);
	static bool CheckInstanceLayer(const char*);

	//debugCallback
	static void CreateDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);
	static void CreateDebugCallback(Context*);
	static void DestroyDebugCallback(Context*);

	//physicsDevice
	static void PickupPhysicalDevice(Context*);

	//logicDevice
	static void CreateLogicDevice(Context*);
	static void GetLogicDeviceQueue(Context*);
	
	//surface
	static void CreateSurface(Context*);
	static void DestroySurface(Context*);

	//memory
	static void CheckPhysicalDeviceMemory(Context*);
};