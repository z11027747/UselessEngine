#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_layout_logic.h"
#include "render/vk/pipeline/pipeline_layout_logic.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "context.h"
#include "base.h"

namespace Render {

	class PipelineSystem final {
	public:

		static void Create(Context* context, const std::string& name) {

			auto pipeline = std::make_shared<Pipeline>();

			DescriptorLayoutLogic::Create(context, pipeline);
			PipelineLayoutLogic::Create(context, pipeline);
			ShaderLogic::Create(context, name, pipeline);
			PipelineLogic::Create(context, pipeline);

			auto pipelineEO = std::make_shared<EngineObject>();
			pipelineEO->AddComponent(pipeline);

			context->renderPipelineEOs.emplace(name, pipelineEO);
		}

		static void Destroy(Context* context) {

			auto& renderPipelineEOs = context->renderPipelineEOs;
			for (auto& kv : renderPipelineEOs) {
				auto& name = kv.first;
				auto& renderPipelineEO = kv.second;

				auto pipeline = renderPipelineEO->GetComponent<Pipeline>();

				DescriptorLayoutLogic::Destroy(context, pipeline);
				PipelineLayoutLogic::Destroy(context, pipeline);
				ShaderLogic::Destroy(context, pipeline);
				PipelineLogic::Destroy(context, pipeline);

				renderPipelineEO->RemoveComponent<Pipeline>();
			}

			context->renderPipelineEOs.clear();
		}

	};

}