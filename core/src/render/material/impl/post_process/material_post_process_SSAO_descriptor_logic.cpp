
#include <random>
#include <vector>
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
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

		VkDescriptorSetLayoutBinding gBufferPosition = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding gBufferNormal = {
			1, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding ssaoUBO = {
			2, // binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(gBufferPosition);
		bindings.push_back(gBufferNormal);
		bindings.push_back(ssaoUBO);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
		graphicsPipeline->subpass = 0;
	}

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

		VkDescriptorImageInfo gBufferPositionImageInfo = {
			global->globalSamplerClamp,
			deferredPass->inputImage2ds[0]->vkImageView,
			deferredPass->inputImage2ds[0]->layout};
		descriptor->imageInfos.push_back(gBufferPositionImageInfo);

		VkDescriptorImageInfo gBufferNormal = {
			global->globalSamplerClamp,
			deferredPass->inputImage2ds[1]->vkImageView,
			deferredPass->inputImage2ds[1]->layout};
		descriptor->imageInfos.push_back(gBufferNormal);

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
									   DescriptorSetLogic::WriteImage(writes, descriptor->set, 0,
																	  bindings[0].descriptorType, descriptor->imageInfos[0]);
									   DescriptorSetLogic::WriteImage(writes, descriptor->set, 1,
																	  bindings[1].descriptorType, descriptor->imageInfos[1]);
									   DescriptorSetLogic::WriteBuffer(writes, descriptor->set, 2,
																	   bindings[2].descriptorType, descriptor->bufferInfos[0]);
								   });

		instance->descriptor = descriptor;

		// update ssao ubo
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
			ssaoUBO.noiseR[i] = glm::vec4(
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