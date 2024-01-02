
#include <array>
#include "common/log_system.h"
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pipeline/shader_test_logic.h"
#include "render/vk/pipeline/shader_skybox_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/camera/camera_comp.h"
#include "common/res_system.h"
#include "context.h"
#include "engine_object.h"

namespace Render
{

	void ShaderLogic::CreateVertexAttrDescriptions(Context *context,
												   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &name = graphicsPipeline->name;
		if (name == "test")
		{
			ShaderTestLogic::CreateVertexAttrDescriptions(context,
														  graphicsPipeline);
		}
		else if (name == "skybox")
		{
			ShaderSkyboxLogic::CreateVertexAttrDescriptions(context,
															graphicsPipeline);
		}
	}

	void ShaderLogic::SetRasterizationCreateInfo(Context *context,
												 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &name = graphicsPipeline->name;
		if (name == "skybox")
		{
			ShaderSkyboxLogic::SetRasterizationCreateInfo(context,
														  graphicsPipeline);
		}
	}

	void ShaderLogic::SetDepthStencilCreateInfo(Context *context,
												std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &name = graphicsPipeline->name;
		if (name == "skybox")
		{
			ShaderSkyboxLogic::SetDepthStencilCreateInfo(context,
														 graphicsPipeline);
		}
	}

	void ShaderLogic::CreateDescriptorSetLayout(Context *context,
												std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding globalUniformBinding0 = {};
		globalUniformBinding0.binding = 0;
		globalUniformBinding0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		globalUniformBinding0.descriptorCount = 1;
		globalUniformBinding0.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		std::vector<VkDescriptorSetLayoutBinding> globalBindings;
		globalBindings.push_back(globalUniformBinding0);

		graphicsPipeline->globalDescriptorSetLayout = DescriptorSetLayoutLogic::Create(context, globalBindings);

		auto &name = graphicsPipeline->name;
		if (name == "test")
		{
			ShaderTestLogic::CreateDescriptorSetLayout(context,
													   graphicsPipeline);
		}
		else if (name == "skybox")
		{
			ShaderSkyboxLogic::CreateDescriptorSetLayout(context,
														 graphicsPipeline);
		}
	}

	void ShaderLogic::DestroyDescriptorSetLayout(Context *context,
												 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalDescriptorSetLayout = graphicsPipeline->globalDescriptorSetLayout;
		DescriptorSetLayoutLogic::Destroy(context, globalDescriptorSetLayout);

		auto &name = graphicsPipeline->name;
		if (name == "test")
		{
			ShaderTestLogic::DestroyDescriptorSetLayout(context,
														graphicsPipeline);
		}
		else if (name == "skybox")
		{
			ShaderSkyboxLogic::DestroyDescriptorSetLayout(context,
														  graphicsPipeline);
		}
	}

	void ShaderLogic::CreateDescriptors(Context *context,
										std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto swapchainImageCount = global->swapchainImageCount;

		auto &globalDescriptorSetLayout = graphicsPipeline->globalDescriptorSetLayout;
		auto globalDescriptorSets = DescriptorSetLogic::Allocate(context,
																 globalDescriptorSetLayout, swapchainImageCount);

		auto globalUBOSize = sizeof(GlobalUBO);

		for (auto i = 0u; i < swapchainImageCount; i++)
		{
			auto globalDescriptor = std::make_shared<Descriptor>();

			auto buffer = BufferLogic::Create(context,
											  globalUBOSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
											  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			VkDescriptorBufferInfo bufferInfo = {
				buffer->vkBuffer,
				0,
				globalUBOSize};

			globalDescriptor->set = globalDescriptorSets[i];
			globalDescriptor->buffer = buffer;
			globalDescriptor->bufferInfo = bufferInfo;

			graphicsPipeline->globalDescriptors.push_back(globalDescriptor);
		}
	}

	void ShaderLogic::DestroyDescriptors(Context *context,
										 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalDescriptors = graphicsPipeline->globalDescriptors;
		for (auto i = 0; i < globalDescriptors.size(); i++)
		{
			auto &globalDescriptor = globalDescriptors[i];

			BufferLogic::Destroy(context, globalDescriptor->buffer);
		}
		globalDescriptors.clear();
	}

	void ShaderLogic::UpdateDescriptorSets(Context *context,
										   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalDescriptors = graphicsPipeline->globalDescriptors;
		for (auto i = 0; i < globalDescriptors.size(); i++)
		{
			auto &globalDescriptor = globalDescriptors[i];

			DescriptorSetLogic::Update(context,
									   [&](std::vector<VkWriteDescriptorSet> &writes)
									   {
										   DescriptorSetLogic::WriteBuffer(context,
																		   writes,
																		   globalDescriptor);
									   });
		}

		auto &name = graphicsPipeline->name;
		if (name == "test")
		{
			ShaderTestLogic::UpdateDescriptorSets(context,
												  graphicsPipeline);
		}
		else if (name == "skybox")
		{
			ShaderSkyboxLogic::UpdateDescriptorSets(context,
													graphicsPipeline);
		}
	}

	void ShaderLogic::UpdateUnitDescriptor(Context *context,
										   std::shared_ptr<Unit> unit)
	{
		auto &name = unit->pipelineName;
		if (name == "test")
		{
			ShaderTestLogic::UpdateUnitDescriptor(context,
												  unit);
		}
		else if (name == "skybox")
		{
			ShaderSkyboxLogic::UpdateUnitDescriptor(context,
													unit);
		}
	}

	void ShaderLogic::CreateUnitDescriptor(Context *context,
										   std::shared_ptr<Unit> unit,
										   std::shared_ptr<Image> image)
	{
		auto &name = unit->pipelineName;
		if (name == "test")
		{
			ShaderTestLogic::CreateUnitDescriptor(context,
												  unit,
												  image);
		}
		else if (name == "skybox")
		{
			ShaderSkyboxLogic::CreateUnitDescriptor(context,
													unit,
													image);
		}
	}

	void ShaderLogic::DestroyUnitDescriptor(Context *context,
											std::shared_ptr<Unit> unit)
	{
		auto &name = unit->pipelineName;
		if (name == "test")
		{
			ShaderTestLogic::DestroyUnitDescriptor(context,
												   unit);
		}
		else if (name == "skybox")
		{
			ShaderSkyboxLogic::DestroyUnitDescriptor(context,
													 unit);
		}
	}

}