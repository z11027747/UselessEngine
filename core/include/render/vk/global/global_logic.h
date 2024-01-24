#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Context;

namespace Render
{
    class InstanceLogic final
    {
    public:
        static void Create(Context *,
                           std::vector<const char *>, bool);
        static void Destroy(Context *);

        static void CreateDebugCallback(Context *);
        static void DestroyDebugCallback(Context *);

        static uint32_t GetApiVersion();
        static bool CheckExtension(const char *);
        static bool CheckLayer(const char *);
    };

    class PhysicalDeviceLogic final
    {
    public:
        static bool Find(Context *);
        static void GetInfo(Context *);

        static bool CheckType(const VkPhysicalDevice &physicalDevice,
                              VkPhysicalDeviceType deviceType);

        static bool CheckQueueFamily(Context *,
                                     const VkPhysicalDevice &,
                                     VkQueueFlags,
                                     uint32_t &);

        static uint32_t FindMemoryType(Context *,
                                       uint32_t,
                                       VkMemoryPropertyFlags);

        static VkFormat FindSupportedFormat(Context *,
                                            const std::vector<VkFormat> &,
                                            VkImageTiling, VkFormatFeatureFlags);

        static VkSurfaceFormatKHR GetFormat(Context *);
        static VkSurfaceCapabilitiesKHR GetCapbilities(Context *);
        static VkPresentModeKHR GetPresentMode(Context *);

        static VkSampleCountFlagBits GetMaxUsableSampleCount(Context *);
    };

    class LogicalDeviceLogic final
    {
    public:
        static void Create(Context *);
        static void Destroy(Context *);
        static VkDevice &Get(Context *);
        static void WaitIdle(Context *);
    };

    class SurfaceLogic final
    {
    public:
        static void Create(Context *);
        static void Destroy(Context *);

        static bool CheckSupport(Context *,
                                 const VkPhysicalDevice &,
                                 uint32_t);
    };

    class SwapchainLogic final
    {
    public:
        static void Create(Context *);
        static void Destroy(Context *);

        static void CreateImageViews(Context *);
        static void DestroyImageViews(Context *);

        static void CreateFences(Context *);
        static void DestroyFences(Context *);

        static void CreateSemaphores(Context *);
        static void DestroySemaphores(Context *);

        static void WaitFence(Context *);
        static uint32_t AcquireImageIndex(Context *);

        static void AllocateCmd(Context *);
        static VkCommandBuffer &BeginCmd(Context *, uint32_t);
        static void EndAndSubmitCmd(Context *, uint32_t);
        static void Present(Context *, uint32_t);
    };

    class DescriptorPoolLogic final
    {
    public:
        static void Create(Context *);
        static void Destroy(Context *);
    };

}