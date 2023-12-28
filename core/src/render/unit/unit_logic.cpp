
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_pool_logic.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "render/unit/unit_comp.h"
#include "render/unit/unit_logic.h"
#include "common/res_system.h"
#include "context.h"
#include "engine_object.h"

namespace Render {

	void UnitLogic::Destroy(Context* context) {
		auto& unitEOs = context->renderUnitEOs;
		for (const auto& unitEO : unitEOs) {
			auto unit = unitEO->GetComponent<Render::Unit>();

			BufferLogic::Destroy(context, unit->vertexBuffer);
			BufferLogic::Destroy(context, unit->indexBuffer);

			ShaderLogic::DestroyUnitDescriptor(context, unit);
		}
		unitEOs.clear();
	}

	void UnitLogic::SetPipelineName(Context* context,
		std::shared_ptr<Unit> unit,
		std::string name
	) {
		unit->pipelineName = name;
	}

	void UnitLogic::SetVertices(Context* context,
		std::shared_ptr<Unit> unit,
		std::vector<Vertex>& vertices
	) {
		auto vertexSize = static_cast<VkDeviceSize>(sizeof(vertices[0]) * vertices.size());

		unit->vertices = vertices;
		unit->vertexBuffer = BufferLogic::Create(context,
			vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BufferLogic::SetVector(context,
			unit->vertexBuffer,
			unit->vertices);
	}

	void UnitLogic::SetIndices(Context* context,
		std::shared_ptr<Unit> unit,
		std::vector<uint16_t>& indices
	) {
		auto indexSize = static_cast<VkDeviceSize>(sizeof(indices[0]) * indices.size());

		unit->indices = indices;
		unit->indexBuffer = BufferLogic::Create(context,
			indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BufferLogic::SetVector(context,
			unit->indexBuffer,
			unit->indices);
	}

	void UnitLogic::SetTexture(Context* context,
		std::shared_ptr<Unit> unit,
		std::string name
	) {
		int w, h;
		unsigned char* data = Common::ResSystem::LoadImg(name, w, h);

		auto imageW = static_cast<uint32_t>(w);
		auto imageH = static_cast<uint32_t>(h);
		auto imageSize = static_cast<VkDeviceSize>(imageW * imageH * 4);

		auto tempBuffer = BufferLogic::CreateTemp(context,
			imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BufferLogic::SetPtr(context,
			tempBuffer,
			data, static_cast<size_t>(imageSize));

		ImageInfo image2dInfo = {
			VK_FORMAT_R8G8B8A8_UNORM, { imageW, imageH, 1 }, VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 0, 1,
		 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_ACCESS_NONE, VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT
		};
		auto image2d = ImageLogic::CreateByInfo(context, image2dInfo);

		ImageLogic::CopyBuffer(context,
			image2d,
			tempBuffer);

		ImageLogic::TransitionLayout(context,
			image2d,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

		Common::ResSystem::FreeImg(data);

		ShaderLogic::CreateUnitDescriptor(context,
			unit,
			image2d);
	}

	void UnitLogic::SetCubeTexture(Context* context,
		std::shared_ptr<Unit> unit,
		std::array<std::string, 6> names
	) {
		uint32_t imageCubeW = 0;
		uint32_t imageCubeH = 0;

		std::vector<unsigned char*> datas(6);

		for (auto i = 0; i < 6; i++) {
			int w, h;
			unsigned char* data = Common::ResSystem::LoadImg(names[i], w, h);

			imageCubeW = static_cast<uint32_t>(w);
			imageCubeH = static_cast<uint32_t>(h);

			datas[i] = data;
		}

		auto imageCubeSizeOne = static_cast<VkDeviceSize>(imageCubeW * imageCubeH * 4);
		auto imageCubeSizeTotal = imageCubeSizeOne * 6;

		auto tempBuffer = BufferLogic::CreateTemp(context,
			imageCubeSizeTotal, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BufferLogic::SetPtrVector(context,
			tempBuffer,
			datas, imageCubeSizeOne);

		ImageInfo imageCubeInfo = {
			VK_FORMAT_R8G8B8A8_UNORM, { imageCubeW, imageCubeH, 1 }, VK_IMAGE_ASPECT_COLOR_BIT,
			//image
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, 6,
			//memory
			 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			 //layout
			 VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			 VK_ACCESS_NONE, VK_ACCESS_TRANSFER_WRITE_BIT,
			 VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT
		};
		auto imageCube = ImageLogic::CreateByInfo(context, imageCubeInfo);

		ImageLogic::CopyBuffer(context,
			imageCube,
			tempBuffer);

		ImageLogic::TransitionLayout(context,
			imageCube,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

		for (auto i = 0; i < 6; i++) {
			Common::ResSystem::FreeImg(datas[i]);
		}

		//TODO
	}

}