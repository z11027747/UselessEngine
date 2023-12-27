#pragma once

#include <string>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_layout_logic.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "context.h"
#include "base.h"

namespace Render {

	class PipelineSystem final {
	public:

		static void Create(Context* context, const std::string& name) {

			auto graphicsPipeline = std::make_shared<GraphicsPipeline>();
			graphicsPipeline->name = name;

			ShaderLogic::CreateModules(context, graphicsPipeline);
			ShaderLogic::CreateVertexAttrDescriptions(context, graphicsPipeline);
			ShaderLogic::CreateDescriptorSetLayout(context, graphicsPipeline);
			ShaderLogic::CreateUniforms(context, graphicsPipeline);
			ShaderLogic::UpdateDescriptorSets(context, graphicsPipeline);
			PipelineLayoutLogic::Create(context, graphicsPipeline);
			PipelineLogic::Create(context, graphicsPipeline);

			auto pipelineEO = std::make_shared<EngineObject>();
			pipelineEO->AddComponent(graphicsPipeline);

			context->renderPipelineEOs[name] = pipelineEO;
		}

		static void Destroy(Context* context) {

			auto& renderPipelineEOs = context->renderPipelineEOs;
			for (const auto& kv : renderPipelineEOs) {
				auto& name = kv.first;
				auto& renderPipelineEO = kv.second;

				auto graphicsPipeline = renderPipelineEO->GetComponent<GraphicsPipeline>();

				PipelineLogic::Destroy(context, graphicsPipeline);
				PipelineLayoutLogic::Destroy(context, graphicsPipeline);
				ShaderLogic::DestroyModules(context, graphicsPipeline);
				ShaderLogic::DestroyDescriptorSetLayout(context, graphicsPipeline);
				ShaderLogic::DestroyUniforms(context, graphicsPipeline);

				renderPipelineEO->RemoveComponent<GraphicsPipeline>();
			}

			renderPipelineEOs.clear();
		}

	};

}