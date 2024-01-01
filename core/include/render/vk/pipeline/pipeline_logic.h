#pragma once

#include <string>
#include <array>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pass/pass_comp.h"

class Context;

namespace Render {

	class PipelineLogic final {
	public:

		static void Create(Context*,
			const std::string&, std::shared_ptr<Pass>);
		static void Destroy(Context*, std::shared_ptr<GraphicsPipeline>);

		static void MakeVertexInputCreateInfo(
			VkVertexInputBindingDescription&,
			std::vector<VkVertexInputAttributeDescription>&,
			VkPipelineVertexInputStateCreateInfo&);
		static void MakeInputAssemblyCreateInfo(
			VkPipelineInputAssemblyStateCreateInfo&);
		static void MakeViewportCreateInfo(
			VkExtent2D&,
			VkViewport&, VkRect2D&,
			VkPipelineViewportStateCreateInfo&);
		static void MakeMultisampleCreateInfo(
			VkPipelineMultisampleStateCreateInfo&);
		static void MakeColorBlendCreateInfo(
			VkPipelineColorBlendAttachmentState&,
			VkPipelineColorBlendStateCreateInfo&);

	};

}