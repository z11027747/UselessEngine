
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/shader_skybox_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "context.h"
#include "engine_object.h"

namespace Render {

	void ShaderSkyboxLogic::CreateDescriptorSetLayout(Context* context,
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

	void ShaderSkyboxLogic::DestroyDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::CreateVertexAttrDescriptions(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::CreateUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::DestroyUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::UpdateDescriptorSets(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::UpdateUniformBuffer(Context* context,
		std::shared_ptr<EngineObject> unitEO,
		std::shared_ptr<Uniform> uniform
	) {
	}

}