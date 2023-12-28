
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

	void ShaderSkyboxLogic::CreateVertexAttrDescriptions(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::CreateDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::DestroyDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::CreateDescriptors(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::DestroyDescriptors(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::UpdateDescriptorSets(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::UpdateDescriptor(Context* context,
		std::shared_ptr<EngineObject> unitEO,
		std::shared_ptr<Descriptor> descriptor
	) {
	}

}