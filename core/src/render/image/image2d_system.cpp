
#include "render/global/global_def.h"
#include "render/device/device_comp.h"
#include "render/device/logical_device_system.h"
#include "render/device/physical_device_system.h"
#include "render/image/image_comp.h"
#include "render/image/image2d_system.h"
#include "context.h"

namespace Render {

	//�����豸�ڴ�(�Դ�)֧�ֵ�ͼ�����
	std::shared_ptr<Image2D> ImageSystem::Create(Context* context,
		VkFormat format, VkExtent2D extent,
		VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags propertiesFlags
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto device = renderGlobalEO->GetComponent<Device>();
		auto& logicalDevice = device->logicalDevice;

		//����vkImage����
		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = format;
		createInfo.extent = { extent.width, extent.height, 1 }; //2d depth=1
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		//VkImageTiling ͼ�����ݸ�ʽ
		//	VK_IMAGE_TILING_LINEAR��������������ķ�ʽ����
		//	VK_IMAGE_TILING_OPTIMAL��������һ�ֶԷ����Ż��ķ�ʽ���У�����Ҫֱ�ӷ��ʾ��������
		createInfo.tiling = tiling;
		createInfo.usage = usage; //����ɫ������
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		//VkImageLayout	ͼ���������ڴ��е����з�ʽ��ʹ�����
		//	VK_IMAGE_LAYOUT_UNDEFINED�� ͼ��ĳ�ʼ����
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkImage vkImage;
		auto ret = vkCreateImage(logicalDevice, &createInfo, nullptr, &vkImage);
		CheckRet(ret, "vkCreateImage");

		//�����ڴ������
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(logicalDevice, vkImage, &requirements);

		auto memoryTypeIndex = PhysicalDeviceSystem::FindMemoryType(context,
			requirements.memoryTypeBits, propertiesFlags);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = requirements.size;
		allocateInfo.memoryTypeIndex = memoryTypeIndex;

		VkDeviceMemory vkDeviceMemory;
		auto allocateRet = vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &vkDeviceMemory);
		CheckRet(allocateRet, "vkAllocateMemory");

		auto bindRet = vkBindImageMemory(logicalDevice, vkImage, vkDeviceMemory, 0);
		CheckRet(bindRet, "vkBindImageMemory");

		auto image2d = std::shared_ptr<Image2D>();
		image2d->fomat = format;
		image2d->extent = extent;
		image2d->vkImage = vkImage;
		image2d->vkDeviceMemory = vkDeviceMemory;
		return image2d;
	}

	void ImageSystem::Destroy(Context* context,
		std::shared_ptr<Image2D> image2d
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto device = renderGlobalEO->GetComponent<Device>();
		auto& logicalDevice = device->logicalDevice;

		vkDestroyImage(logicalDevice, image2d->vkImage, nullptr);
		vkFreeMemory(logicalDevice, image2d->vkDeviceMemory, nullptr);

		if (image2d->vkImageView != nullptr) {
			vkDestroyImageView(logicalDevice, image2d->vkImageView, nullptr);
		}
	}

	//�κ�VkImage���󣬶���Ҫ����һ��VkImageView�������󶨷�����
	void ImageSystem::CreateImageView(Context* context,
		std::shared_ptr<Image2D> image2d,
		VkImageAspectFlags aspectMask
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto device = renderGlobalEO->GetComponent<Device>();
		auto& logicalDevice = device->logicalDevice;

		auto format = image2d->fomat;
		auto& vkImage = image2d->vkImage;

		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = vkImage;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;

		//����ͼ����ɫͨ����ӳ��
		auto identity = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components = { identity,identity,identity,identity };

		//ָ��ͼ�����;��ͼ�����һ���ֿ��Ա�����
		createInfo.subresourceRange = { aspectMask,0,1,0,1 };

		VkImageView vkImageView;
		auto ret = vkCreateImageView(logicalDevice, &createInfo, nullptr, &vkImageView);
		CheckRet(ret, "vkCreateImageView");

		image2d->vkImageView = vkImageView;
	}

}