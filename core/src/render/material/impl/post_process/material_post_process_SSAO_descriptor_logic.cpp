
#include <random>
#include <vector>
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/material/impl/material_post_process_logic.h"
#include "render/material/material_logic.h"
#include "render/post_process/post_process_comp.h"
#include "context.hpp"

namespace Render
{
	void MaterialPostProcessSSAODescriptorLogic::CreateSetLayout(Context *context,
																 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		VkDescriptorSetLayoutBinding gBufferPositionDepth = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding gBufferNormal = {
			1, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding gBufferDepth = {
			2, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding ssaoUBO = {
			3, // binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(gBufferPositionDepth);
		bindings.push_back(gBufferNormal);
		bindings.push_back(gBufferDepth);
		bindings.push_back(ssaoUBO);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
		graphicsPipeline->subpass = 0;
	}

	constexpr int imageCount = 3; // gBuffer position+normal+depth
	static float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
	void MaterialPostProcessSSAODescriptorLogic::AllocateAndUpdate(Context *context,
																   std::shared_ptr<MaterialInstance> instance)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
		auto &postProcessPipeline = global->pipelineMap[Define::Pipeline::PostProcess_SSAO];

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, postProcessPipeline->descriptorSetLayout);
		descriptor->set = descriptorSet;

		auto &deferredPass = global->passMap[Define::Pass::Deferred];

		VkDescriptorImageInfo gBufferPositionDepthImageInfo = {
			global->globalSamplerClamp,
			deferredPass->inputImage2ds[0]->vkImageView,
			deferredPass->inputImage2ds[0]->layout};
		descriptor->imageInfos.push_back(gBufferPositionDepthImageInfo);

		VkDescriptorImageInfo gBufferNormalImageInfo = {
			global->globalSamplerClamp,
			deferredPass->inputImage2ds[1]->vkImageView,
			deferredPass->inputImage2ds[1]->layout};
		descriptor->imageInfos.push_back(gBufferNormalImageInfo);

		VkDescriptorImageInfo gBufferDepthImageInfo = {
			global->globalSamplerClamp,
			deferredPass->inputImage2ds[3]->vkImageView,
			deferredPass->inputImage2ds[3]->layout};
		descriptor->imageInfos.push_back(gBufferDepthImageInfo);

		if (instance->buffer != nullptr)
			BufferLogic::Destroy(context, instance->buffer);

		// ubo
		auto uboSize = sizeof(PostProcess_SSAOUBO);
		MaterialInstanceLogic::CreateBuffer(context, instance, uboSize);

		VkDescriptorBufferInfo bufferInfo = {
			instance->buffer->vkBuffer,
			0,
			instance->buffer->size};
		descriptor->bufferInfos.push_back(bufferInfo);

		DescriptorSetLogic::Update(context,
								   [=](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   auto &bindings = postProcessPipeline->descriptorBindings;
									   for (auto imageIdx = 0; imageIdx < imageCount; imageIdx++)
									   {
										   DescriptorSetLogic::WriteImage(writes, descriptor->set, imageIdx,
																		  bindings[imageIdx].descriptorType, descriptor->imageInfos[imageIdx]);
									   }
									   auto bufferIdx = imageCount;
									   DescriptorSetLogic::WriteBuffer(writes, descriptor->set, bufferIdx,
																	   bindings[bufferIdx].descriptorType, descriptor->bufferInfos[0]);
								   });

		instance->descriptor = descriptor;

		// update ssaoUBO
		PostProcess_SSAOUBO ssaoUBO = {};

		// Sample kernel
		std::uniform_real_distribution<float> rndDist(0.0f, 1.0f);
		std::default_random_engine rndEngine;
		for (auto i = 0; i < ssaoKernelSize; ++i)
		{
			glm::vec3 sample = {
				rndDist(rndEngine) * 2.0 - 1.0,
				rndDist(rndEngine) * 2.0 - 1.0,
				rndDist(rndEngine) // 半球
			};
			sample = glm::normalize(sample);
			sample *= rndDist(rndEngine);

			auto scale = float(i) / float(ssaoKernelSize);
			scale = lerp(0.1f, 1.0f, scale * scale);

			ssaoUBO.samples[i] = glm::vec4(sample * scale, 0.0f);
		}

		// Random noise
		for (auto i = 0; i < ssaoNoiseDim * ssaoNoiseDim; i++)
		{
			ssaoUBO.noiseValues[i] = glm::vec4(
				rndDist(rndEngine) * 2.0f - 1.0f,
				rndDist(rndEngine) * 2.0f - 1.0f,
				0.0f,
				0.0f);
		}

		auto &buffer = instance->buffer;
		BufferSetLogic::Set(context,
							buffer,
							ssaoUBO);
	}
	void MaterialPostProcessSSAODescriptorLogic::Destroy(Context *context,
														 std::shared_ptr<MaterialInstance> materialInstance)
	{
	}
}