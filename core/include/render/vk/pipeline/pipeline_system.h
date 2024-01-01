#pragma once

#include <string>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_layout_logic.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "logic/camera/camera_comp.h"
#include "context.h"
#include "engine_object.h"

namespace Render {

	class PipelineSystem final {
	public:

		static void Create(Context* context,
			const std::string& name,
			std::shared_ptr<Logic::Camera> camera
		) {
			auto graphicsPipeline = std::make_shared<GraphicsPipeline>();
			graphicsPipeline->name = name;

			//ShaderLogic::CreateModules(context, graphicsPipeline);
			//ShaderLogic::CreateVertexAttrDescriptions(context, graphicsPipeline);

		}

		static void DestroyAll(Context* context) {
			auto& pipelines = context->renderPipelines;
			for (const auto& kv : pipelines) {
				auto& name = kv.first;
				auto& graphicsPipeline = kv.second;

				PipelineLogic::Destroy(context, graphicsPipeline);
				PipelineLayoutLogic::Destroy(context, graphicsPipeline);
				ShaderLogic::DestroyModules(context, graphicsPipeline);
				ShaderLogic::DestroyDescriptorSetLayout(context, graphicsPipeline);
				ShaderLogic::DestroyDescriptors(context, graphicsPipeline);
			}
			pipelines.clear();
		}

	};

}