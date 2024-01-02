#pragma once

#include <memory>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/unit/unit_comp.h"

class Context;
class EngineObject;

namespace Render
{
	class ShaderLogic final
	{
	public:
		static void CreateVertexAttrDescriptions(Context *, std::shared_ptr<GraphicsPipeline>);
		static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
		static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);

		static void CreateDescriptorSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
		static void DestroyDescriptorSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);

		static void CreateDescriptors(Context *, std::shared_ptr<GraphicsPipeline>);
		static void DestroyDescriptors(Context *, std::shared_ptr<GraphicsPipeline>);

		static void UpdateDescriptorSets(Context *, std::shared_ptr<GraphicsPipeline>);

		static void UpdateUnitDescriptor(Context *, std::shared_ptr<Unit>);
		static void CreateUnitDescriptor(Context *, std::shared_ptr<Unit>, std::shared_ptr<Image>);
		static void DestroyUnitDescriptor(Context *, std::shared_ptr<Unit>);
	};
}