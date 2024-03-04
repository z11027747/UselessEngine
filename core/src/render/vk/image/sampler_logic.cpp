
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/image/sampler_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	VkSampler SamplerLogic::Create(Context *context, bool isLinear, bool isClamp,
								   uint32_t minMipLevels, uint32_t maxMipLevels)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;

		VkSamplerCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		if (isLinear)
		{
			createInfo.magFilter = VK_FILTER_LINEAR;
			createInfo.minFilter = VK_FILTER_LINEAR;
		}
		else
		{
			createInfo.magFilter = VK_FILTER_NEAREST;
			createInfo.minFilter = VK_FILTER_NEAREST;
		}
		if (isClamp)
		{
			createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		}
		else
		{
			createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
		createInfo.anisotropyEnable = false;
		createInfo.maxAnisotropy = 1;
		createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
		createInfo.unnormalizedCoordinates = false;
		createInfo.compareEnable = false;
		createInfo.compareOp = VK_COMPARE_OP_NEVER;
		createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		createInfo.mipLodBias = 0.0f;
		createInfo.minLod = static_cast<float>(minMipLevels);
		createInfo.maxLod = static_cast<float>(maxMipLevels);

		VkSampler sampler;
		auto ret = vkCreateSampler(logicalDevice, &createInfo, nullptr, &sampler);
		CheckRet(ret, "vkCreateSampler");

		return sampler;
	}

	VkSampler SamplerLogic::CreateDepth(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
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
		createInfo.compareEnable = true;
		createInfo.compareOp = VK_COMPARE_OP_LESS;
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
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;

		vkDestroySampler(logicalDevice, sampler, nullptr);
	}
}