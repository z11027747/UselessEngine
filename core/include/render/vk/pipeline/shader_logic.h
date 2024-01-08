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
		static void SetViewport(Context *, std::shared_ptr<GraphicsPipeline>);
		static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
		static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);

		static void CreateGlobalDescriptorSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
		static void DestroyGlobalDescriptorSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
		static void CreateGlobalDescriptor(Context *, std::shared_ptr<GraphicsPipeline>);
		static void DestroyGlobalDescriptor(Context *, std::shared_ptr<GraphicsPipeline>);
		static void UpdateGlobalDescriptor(Context *, std::shared_ptr<GraphicsPipeline>);

		static void CreateDescriptorSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
		static void DestroyDescriptorSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);

		static void CreateUnitDescriptor(Context *, std::shared_ptr<Unit>);
		static void DestroyUnitDescriptor(Context *, std::shared_ptr<Unit>);
		static void UpdateUnitDescriptor(Context *, std::shared_ptr<Unit>, uint32_t);
	};
}