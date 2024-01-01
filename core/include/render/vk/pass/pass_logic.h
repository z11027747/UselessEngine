#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"

class Context;

namespace Render {

	class PassLogic final {
	public:
		static void CreateMain(Context*);
		static void DestroyAll(Context*);
	};

	class RenderPassLogic final {
	public:
		static void CreateColorAttachment(Context*, std::shared_ptr<Pass>);
		static void CreateDepthAttachment(Context*, std::shared_ptr<Pass>);

		static void CreatColorImage2dsBySwapchain(Context*, std::shared_ptr<Pass>);
		static void CreateDepthImage2ds(Context*, std::shared_ptr<Pass>);

		static void AddSubPass(Context*, std::shared_ptr<Pass>);

		static void Create(Context*, std::shared_ptr<Pass>);
		static void Destroy(Context*, std::shared_ptr<Pass>);
	};

	class FramebufferLogic final {
	public:
		static void Create(Context*, std::shared_ptr<Pass>);
		static void Destroy(Context*, std::shared_ptr<Pass>);

		static void BeginCmd(Context*, uint32_t, VkCommandBuffer&);
		static void EndCmd(Context*, uint32_t, VkCommandBuffer&);

		static void BeginRenderPass(Context*, uint32_t, std::shared_ptr<Pass>);
		static void EndRenderPass(Context*, uint32_t, std::shared_ptr<Pass>);

		static void RenderUnits(Context*,
			std::shared_ptr<Framebuffer>, uint32_t,
			GlobalUBO&);
	};

}