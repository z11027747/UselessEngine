#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/swapchain/swapchain_comp.h"

class Context;

namespace Render {

	class SwapchainSurfaceSystem final {
	public:

		//�������洰��
		static void Create(Context*,
			std::shared_ptr<Swapchain> swapchain
		);

		//���ٱ��洰��
		static void Destroy(Context*,
			std::shared_ptr<Swapchain> swapchain
		);

		//���������Ƿ�֧�ֱ��洰��
		static bool CheckSupport(Context*,
			const VkPhysicalDevice& physicalDevice,
			uint32_t queueFamilyIndex
		);

		//��ȡ����֧�ֵĸ�ʽ
		static VkSurfaceFormatKHR GetFormat(Context*);
		//��ȡ������õĳ���ģʽ
		static VkPresentModeKHR GetPresentMode(Context*);
		//��ȡ����֧�ֵ�����
		static VkSurfaceCapabilitiesKHR GetCapbilities(Context*);


	};


}