
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_pool_logic.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image2d_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "render/unit/unit_comp.h"
#include "render/unit/unit_logic.h"
#include "common/res_system.h"
#include "context.h"
#include "base.h"

namespace Render {

	void UnitLogic::Destroy(Context* context) {
		auto& renderUnitEOs = context->renderUnitEOs;
		for (const auto& renderUnitEO : renderUnitEOs) {
			auto unit = renderUnitEO->GetComponent<Render::Unit>();

			BufferLogic::Destroy(context,
				unit->vertexBuffer);

			BufferLogic::Destroy(context,
				unit->indexBuffer);

			Image2DLogic::Destroy(context,
				unit->image2D);
		}
		renderUnitEOs.clear();
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

		BufferLogic::SetValues(context,
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

		BufferLogic::SetValues(context,
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

		Image2DInfo info = {
			VK_FORMAT_R8G8B8A8_SRGB,
			{ imageW, imageH },
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_ACCESS_NONE, VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT
		};
		auto image2d = Image2DLogic::CreateByInfo(context, info);

		Image2DLogic::CopyBuffer(context,
			image2d,
			tempBuffer);

		Image2DLogic::TransitionLayout(context,
			image2d,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

		Common::ResSystem::FreeImg(data);

		unit->image2D = image2d;
	}

}