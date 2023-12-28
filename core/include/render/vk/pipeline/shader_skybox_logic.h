#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/vk/pipeline/pipeline_comp.h"

class Context;
class EngineObject;

namespace Render {

	class ShaderSkyboxLogic final {
	public:

		static void CreateVertexAttrDescriptions(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void CreateDescriptorSetLayout(Context*,
			std::shared_ptr<GraphicsPipeline>);
		static void DestroyDescriptorSetLayout(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void CreateDescriptors(Context*,
			std::shared_ptr<GraphicsPipeline>);
		static void DestroyDescriptors(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void UpdateDescriptorSets(Context*,
			std::shared_ptr<GraphicsPipeline>);

		static void UpdateDescriptor(Context*,
			std::shared_ptr<EngineObject>,
			std::shared_ptr<Descriptor>);

	};

}