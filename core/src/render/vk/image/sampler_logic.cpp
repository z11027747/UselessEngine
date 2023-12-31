
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/image/sampler_logic.h"
#include "context.h"

namespace Render
{

	VkSampler SamplerLogic::Create(Context *context, bool forDepth)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		VkSamplerCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.magFilter = VK_FILTER_LINEAR;
		createInfo.minFilter = VK_FILTER_LINEAR;
		createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		createInfo.anisotropyEnable = false;
		createInfo.maxAnisotropy = 1;
		createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
		createInfo.unnormalizedCoordinates = false;
		if (forDepth)
		{
			createInfo.compareEnable = true;
			createInfo.compareOp = VK_COMPARE_OP_LESS;
		}
		else
		{
			createInfo.compareEnable = false;
			createInfo.compareOp = VK_COMPARE_OP_NEVER;
		}
		createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		createInfo.mipLodBias = 0.0f;
		createInfo.minLod = 0.0f;
		createInfo.maxLod = 1.0f;

		VkSampler sampler;
		auto ret = vkCreateSampler(logicalDevice, &createInfo, nullptr, &sampler);
		CheckRet(ret, "vkCreateSampler");

		return sampler;
	}

	void SamplerLogic::Destroy(Context *context, VkSampler &sampler)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		vkDestroySampler(logicalDevice, sampler, nullptr);
	}

}