﻿
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	void FramebufferLogic::GetSwapchainImage2ds(Context *context,
												std::shared_ptr<Pass> pass)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &swapchainImages = global->swapchainImages;

		pass->isGetSwapchainImage = true;
		pass->colorImage2ds.insert(pass->colorImage2ds.end(), swapchainImages.begin(), swapchainImages.end());
	}

	void FramebufferLogic::CreateColorImage2d(Context *context,
											  std::shared_ptr<Pass> pass,
											  VkSampleCountFlagBits samplers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
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
		auto global = globalEO->GetComponent<Global>();
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

		pass->depthImage2ds.push_back(depthImage2d);
	}

	void FramebufferLogic::CreateDepthStencilImage2d(Context *context,
													 std::shared_ptr<Pass> pass,
													 VkSampleCountFlagBits samplers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto depthStencilFormat = global->depthStencilFormat;

		ImageCreateInfo imageCreateInfo = {
			depthStencilFormat, {pass->extent.width, pass->extent.height, 0}, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
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
			VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL};
		auto depthImage2d = ImageLogic::CreateByInfo(context,
													 imageCreateInfo);

		pass->depthImage2ds.push_back(depthImage2d);
	}

	void FramebufferLogic::CreateResolveImage2d(Context *context,
												std::shared_ptr<Pass> pass,
												VkSampleCountFlagBits samplers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
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

	void FramebufferLogic::CreateInputImage2d(Context *context,
											  std::shared_ptr<Pass> pass,
											  VkFormat format, VkSampleCountFlagBits samplers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		ImageCreateInfo imageCreateInfo = {
			format, {pass->extent.width, pass->extent.height, 0}, VK_IMAGE_ASPECT_COLOR_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
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
		auto inputImage2d = ImageLogic::CreateByInfo(context,
													 imageCreateInfo);

		pass->inputImage2ds.push_back(inputImage2d);
	}

	void FramebufferLogic::CreateBlitImage2d(Context *context,
											 std::shared_ptr<Pass> pass,
											 uint32_t mipLevels)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto surfaceFormat = global->surfaceFormat;

		ImageCreateInfo imageCreateInfo = {
			surfaceFormat.format, {pass->extent.width, pass->extent.height, 0}, VK_IMAGE_ASPECT_COLOR_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL | VK_IMAGE_USAGE_SAMPLED_BIT,
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
		auto &depthImage2ds = pass->depthImage2ds;
		for (const auto &depthImage2d : depthImage2ds)
		{
			ImageLogic::Destroy(context, depthImage2d);
		}
		depthImage2ds.clear();
	}

	void FramebufferLogic::DestroyResolveImage2d(Context *context,
												 std::shared_ptr<Pass> pass)
	{
		if (pass->resolveImage2d != nullptr)
		{
			ImageLogic::Destroy(context, pass->resolveImage2d);
		}
	}

	void FramebufferLogic::DestroyInputImage2ds(Context *context,
												std::shared_ptr<Pass> pass)
	{
		auto &inputImage2ds = pass->inputImage2ds;
		for (const auto &inputImage2d : inputImage2ds)
		{
			ImageLogic::Destroy(context, inputImage2d);
		}
		inputImage2ds.clear();
	}
}