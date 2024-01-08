
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/shader_impl/shader_skybox_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "context.h"
#include "engine_object.h"

namespace Render
{

	void ShaderSkyboxLogic::CreateVertexAttrDescriptions(Context *context,
														 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkVertexInputAttributeDescription positionOSDescription0 = {};
		positionOSDescription0.location = 0;
		positionOSDescription0.binding = 0;
		positionOSDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
		positionOSDescription0.offset = 0;

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription0};
	}

	void ShaderSkyboxLogic::SetRasterizationCreateInfo(Context *context,
													   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
		// rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	}

	void ShaderSkyboxLogic::SetDepthStencilCreateInfo(Context *context,
													  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &depthStencilStateCreateInfo = stageInfo.depthStencilStateCreateInfo;
		depthStencilStateCreateInfo.depthTestEnable = true;
		depthStencilStateCreateInfo.depthWriteEnable = false;
	}

	void ShaderSkyboxLogic::CreateDescriptorSetLayout(Context *context,
													  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding samplerBinding0 = {};
		samplerBinding0.binding = 0;
		samplerBinding0.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerBinding0.descriptorCount = 1;
		samplerBinding0.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(samplerBinding0);

		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}

	void ShaderSkyboxLogic::DestroyDescriptorSetLayout(Context *context,
													   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
		DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
	}

	void ShaderSkyboxLogic::CreateUnitDescriptor(Context *context,
												 std::shared_ptr<Unit> unit)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &graphicsPipeline = global->pipelines[unit->pipelineName];
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);
		auto sampler = SamplerLogic::Create(context);

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->set = descriptorSet;

		VkDescriptorImageInfo vkImageCubeInfo = {
			sampler,
			unit->image0->vkImageView,
			unit->image0->layout};
		descriptor->vkImage0Info = vkImageCubeInfo;

		unit->descriptor = descriptor;

		DescriptorSetLogic::Update(context,
								   [&](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   DescriptorSetLogic::WriteImage(context,
																	  writes,
																	  unit->descriptor);
								   });
	}

	void ShaderSkyboxLogic::DestroyUnitDescriptor(Context *context,
												  std::shared_ptr<Unit> unit)
	{
		auto &descriptor = unit->descriptor;
		ImageLogic::Destroy(context, unit->image0);
		SamplerLogic::Destroy(context, descriptor->vkImage0Info.sampler);
	}

}