
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/shader_test_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image2d_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "context.h"
#include "base.h"

namespace Render {

	void ShaderTestLogic::CreateDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		std::vector<VkDescriptorSetLayoutBinding> bindings;

		VkDescriptorSetLayoutBinding uniformBinding0 = {};
		uniformBinding0.binding = 0;
		uniformBinding0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uniformBinding0.descriptorCount = 1;
		uniformBinding0.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		bindings.push_back(uniformBinding0);

		VkDescriptorSetLayoutBinding uniformBinding1 = {};
		uniformBinding1.binding = 1;
		uniformBinding1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uniformBinding1.descriptorCount = 1;
		uniformBinding1.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		bindings.push_back(uniformBinding1);

		VkDescriptorSetLayoutBinding samplerBinding2 = {};
		samplerBinding2.binding = 2;
		samplerBinding2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerBinding2.descriptorCount = 1;
		samplerBinding2.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings.push_back(samplerBinding2);

		VkDescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		createInfo.pBindings = bindings.data();

		VkDescriptorSetLayout descriptorSetLayout;
		auto ret = vkCreateDescriptorSetLayout(logicalDevice, &createInfo, nullptr, &descriptorSetLayout);
		CheckRet(ret, "vkCreateDescriptorSetLayout");

		graphicsPipeline->descriptorSetLayout = descriptorSetLayout;
	}

	void ShaderTestLogic::DestroyDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
		vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
	}

	void ShaderTestLogic::CreateVertexAttrDescriptions(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		VkVertexInputAttributeDescription positionOSDescription0 = {};
		positionOSDescription0.location = 0;
		positionOSDescription0.binding = 0;
		positionOSDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
		positionOSDescription0.offset = offsetof(Render::Vertex, positionOS);

		VkVertexInputAttributeDescription colorDescription1 = {};
		colorDescription1.location = 1;
		colorDescription1.binding = 0;
		colorDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
		colorDescription1.offset = offsetof(Render::Vertex, color);

		VkVertexInputAttributeDescription uv0Description2 = {};
		uv0Description2.location = 2;
		uv0Description2.binding = 0;
		uv0Description2.format = VK_FORMAT_R32G32_SFLOAT;
		uv0Description2.offset = offsetof(Render::Vertex, uv0);

		graphicsPipeline->vertexAttrDescriptions = {
			positionOSDescription0,
			colorDescription1,
			uv0Description2,
		};
	}

	void ShaderTestLogic::CreateUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;
		auto& descriptorPool = global->descriptorPool;

		auto& descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = descriptorPool;
		allocateInfo.descriptorSetCount = maxFrameInFlight;

		std::vector<VkDescriptorSetLayout> layouts(maxFrameInFlight, descriptorSetLayout);
		allocateInfo.pSetLayouts = layouts.data();

		std::vector<VkDescriptorSet> descriptorSets(maxFrameInFlight);
		auto ret = vkAllocateDescriptorSets(logicalDevice, &allocateInfo, descriptorSets.data());
		CheckRet(ret, "vkAllocateDescriptorSets");

		auto uniform0Size = (sizeof(CameraUBO));
		auto uniform1Size = (sizeof(TransformUBO));

		for (auto i = 0u; i < maxFrameInFlight; i++) {
			auto uniformTest = std::make_shared<UniformTest>();

			uniformTest->descriptorSet = descriptorSets[i];

			uniformTest->uniform0Buffer = BufferLogic::Create(context,
				uniform0Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			uniformTest->uniform1Buffer = BufferLogic::Create(context,
				uniform1Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			uniformTest->sampler = SamplerLogic::Create(context);

			graphicsPipeline->uniforms.push_back(uniformTest);
		}
	}

	void ShaderTestLogic::DestroyUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& uniforms = graphicsPipeline->uniforms;
		for (auto& uniform : uniforms) {
			auto uniformTest = std::dynamic_pointer_cast<UniformTest>(uniform);

			BufferLogic::Destroy(context,
				uniformTest->uniform0Buffer);

			BufferLogic::Destroy(context,
				uniformTest->uniform1Buffer);

			SamplerLogic::Destroy(context,
				uniformTest->sampler);
		}
		uniforms.clear();
	}

	void ShaderTestLogic::UpdateDescriptorSets(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		auto uniform0Size = (sizeof(CameraUBO));
		auto uniform1Size = (sizeof(TransformUBO));

		for (auto i = 0u; i < maxFrameInFlight; i++) {
			auto& uniform = graphicsPipeline->uniforms[i];
			auto uniformTest = std::dynamic_pointer_cast<UniformTest>(uniform);

			std::vector<VkWriteDescriptorSet> writes;

			VkWriteDescriptorSet uniformWrite0 = {};
			uniformWrite0.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			uniformWrite0.dstSet = uniformTest->descriptorSet;
			uniformWrite0.dstBinding = 0;
			uniformWrite0.dstArrayElement = 0;
			uniformWrite0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uniformWrite0.descriptorCount = 1;

			VkDescriptorBufferInfo uniformBuffer0 = {};
			uniformBuffer0.buffer = uniformTest->uniform0Buffer->vkBuffer;
			uniformBuffer0.offset = 0;
			uniformBuffer0.range = uniform0Size;
			uniformWrite0.pBufferInfo = &uniformBuffer0;
			writes.push_back(uniformWrite0);

			VkWriteDescriptorSet uniformWrite1 = {};
			uniformWrite1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			uniformWrite1.dstSet = uniformTest->descriptorSet;
			uniformWrite1.dstBinding = 1;
			uniformWrite1.dstArrayElement = 0;
			uniformWrite1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uniformWrite1.descriptorCount = 1;

			VkDescriptorBufferInfo uniformBuffer1 = {};
			uniformBuffer1.buffer = uniformTest->uniform1Buffer->vkBuffer;
			uniformBuffer1.offset = 0;
			uniformBuffer1.range = uniform1Size;
			uniformWrite1.pBufferInfo = &uniformBuffer1;
			writes.push_back(uniformWrite1);

			vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
		}
	}

	void ShaderTestLogic::UpdateUniformBuffer(Context* context,
		std::shared_ptr<EngineObject> unitEO,
		std::shared_ptr<Uniform> uniform
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto uniformTest = std::dynamic_pointer_cast<UniformTest>(uniform);

		auto& cameraEO = context->cameraEO;
		auto camera = cameraEO->GetComponent<Logic::Camera>();

		CameraUBO cameraUBO = {
			camera->view,
			camera->projection
		};

		BufferLogic::SetValue(context,
			uniformTest->uniform0Buffer,
			cameraUBO);

		auto transform = unitEO->GetComponent<Logic::Transform>();

		auto model = glm::mat4(1.0f);
		model = glm::translate(model, transform->position);
		model = glm::rotate(model, glm::radians(transform->eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform->eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform->eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, transform->scale);

		TransformUBO transformUBO = {
			model
		};

		BufferLogic::SetValue(context,
			uniformTest->uniform1Buffer,
			transformUBO);

		auto unit = unitEO->GetComponent<Render::Unit>();
		auto& image2D = unit->image2D;

		VkWriteDescriptorSet samplerWrite2 = {};
		samplerWrite2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		samplerWrite2.dstSet = uniformTest->descriptorSet;
		samplerWrite2.dstBinding = 2;
		samplerWrite2.dstArrayElement = 0;
		samplerWrite2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerWrite2.descriptorCount = 1;

		VkDescriptorImageInfo samplerImage2 = {};
		samplerImage2.sampler = uniformTest->sampler;
		samplerImage2.imageView = image2D->vkImageView;
		samplerImage2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		samplerWrite2.pImageInfo = &samplerImage2;

		vkUpdateDescriptorSets(logicalDevice, 1, &samplerWrite2, 0, nullptr);
	}

}