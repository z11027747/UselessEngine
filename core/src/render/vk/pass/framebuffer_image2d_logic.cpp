
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void FramebufferLogic::GetSwapchainImage2ds(Context *context,
												std::shared_ptr<Pass> pass)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto swapchainImageCount = global->swapchainImageCount;
		auto &swapchainImages = global->swapchainImages;

		for (auto i = 0u; i < swapchainImageCount; i++)
		{
			pass->colorImage2ds.push_back(swapchainImages[i]);
		}
		pass->isGetSwapchainImage = true;
	}

	void FramebufferLogic::CreateColorImage2d(Context *context,
											  std::shared_ptr<Pass> pass,
											  VkSampleCountFlagBits samplers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto surfaceFormat = global->surfaceFormat;

		ImageCreateInfo imageCreateInfo = {
			surfaceFormat.format, {pass->extent.width, pass->extent.height, 0}, VK_IMAGE_ASPECT_COLOR_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			1,
			VK_IMAGE_VIEW_TYPE_2D,
			1,
			samplers,
			// memory
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			// layout
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		auto colorImage2d = ImageLogic::CreateByInfo(context,
													 imageCreateInfo);

		pass->colorImage2ds.push_back(colorImage2d);
	}

	void FramebufferLogic::CreateDepthImage2d(Context *context,
											  std::shared_ptr<Pass> pass,
											  VkSampleCountFlagBits samplers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto depthFormat = global->depthFormat;

		ImageCreateInfo imageCreateInfo = {
			depthFormat, {pass->extent.width, pass->extent.height, 0}, VK_IMAGE_ASPECT_DEPTH_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			1,
			VK_IMAGE_VIEW_TYPE_2D,
			1,
			samplers,
			// memory
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			// layout
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL};

		auto depthImage2d = ImageLogic::CreateByInfo(context,
													 imageCreateInfo);

		pass->depthImage2d = depthImage2d;
	}

	void FramebufferLogic::CreateResolveImage2d(Context *context,
												std::shared_ptr<Pass> pass,
												VkSampleCountFlagBits samplers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto surfaceFormat = global->surfaceFormat;

		ImageCreateInfo imageCreateInfo = {
			surfaceFormat.format, {pass->extent.width, pass->extent.height, 0}, VK_IMAGE_ASPECT_COLOR_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			1,
			VK_IMAGE_VIEW_TYPE_2D,
			1,
			samplers,
			// memory
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			// layout
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};

		auto resolveImage2d = ImageLogic::CreateByInfo(context,
													   imageCreateInfo);

		pass->resolveImage2d = resolveImage2d;
	}

	void FramebufferLogic::CreatePostProcessImage2d(Context *context,
													std::shared_ptr<Pass> pass, uint32_t mipLevels)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto surfaceFormat = global->surfaceFormat;

		ImageCreateInfo imageCreateInfo = {
			surfaceFormat.format, {pass->extent.width, pass->extent.height, 0}, VK_IMAGE_ASPECT_COLOR_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			1,
			VK_IMAGE_VIEW_TYPE_2D,
			mipLevels,
			VK_SAMPLE_COUNT_1_BIT,
			// memory
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			// layout
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		auto colorImage2d = ImageLogic::CreateByInfo(context,
													 imageCreateInfo);

		pass->colorImage2ds.push_back(colorImage2d);
	}

	void FramebufferLogic::DestroyColorImage2ds(Context *context,
												std::shared_ptr<Pass> pass)
	{
		if (pass->isGetSwapchainImage)
		{
			pass->colorImage2ds.clear();
			return;
		}

		auto &colorImage2ds = pass->colorImage2ds;
		for (const auto &colorImage2d : colorImage2ds)
		{
			ImageLogic::Destroy(context, colorImage2d);
		}
		colorImage2ds.clear();
	}

	void FramebufferLogic::DestroyDepthImage2d(Context *context,
											   std::shared_ptr<Pass> pass)
	{
		if (pass->depthImage2d != nullptr)
		{
			ImageLogic::Destroy(context, pass->depthImage2d);
		}
	}

	void FramebufferLogic::DestroyResolveImage2d(Context *context,
												 std::shared_ptr<Pass> pass)
	{
		if (pass->resolveImage2d != nullptr)
		{
			ImageLogic::Destroy(context, pass->resolveImage2d);
		}
	}
}