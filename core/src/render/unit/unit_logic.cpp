
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_pool_logic.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/unit/unit_comp.h";
#include "render/unit/unit_logic.h";
#include "context.h"
#include "base.h"

namespace Render {

	void UnitLogic::SetVertices(Context* context,
		std::shared_ptr<Unit> unit,
		std::vector<Vertex>& vertices) {

		unit->vertices = vertices;

		auto& vertexBuffer = unit->vertexBuffer;
		if (vertexBuffer != nullptr) {

			BufferLogic::Destroy(context,
				vertexBuffer);
		}

		auto vertexSize = static_cast<VkDeviceSize>(sizeof(vertices[0]) * vertices.size());

		unit->vertexBuffer = BufferLogic::Create(context,
			vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	void UnitLogic::SetIndices(Context* context,
		std::shared_ptr<Unit> unit,
		std::vector<uint16_t>& indices) {

		unit->indices = indices;

		auto& indexBuffer = unit->indexBuffer;
		if (indexBuffer != nullptr) {

			BufferLogic::Destroy(context,
				indexBuffer);
		}

		auto indexSize = static_cast<VkDeviceSize>(sizeof(indices[0]) * indices.size());

		unit->indexBuffer = BufferLogic::Create(context,
			indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	void UnitLogic::UpdateBuffers(Context* context,
		std::shared_ptr<Unit> unit) {
		UpdateVertexBuffer(context, unit);
		UpdateIndexBuffer(context, unit);
	}

	void UnitLogic::UpdateVertexBuffer(Context* context,
		std::shared_ptr<Unit> unit) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto& global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		BufferLogic::SetValues(context,
			unit->vertexBuffer,
			unit->vertices);
	}

	void UnitLogic::UpdateIndexBuffer(Context* context,
		std::shared_ptr<Unit> unit) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto& global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		BufferLogic::SetValues(context,
			unit->indexBuffer,
			unit->indices);
	}

	void UnitLogic::Destroy(Context* context) {

		auto& renderUnitEOs = context->renderUnitEOs;
		for (const auto& renderUnitEO : renderUnitEOs) {

			auto unit = renderUnitEO->GetComponent<Render::Unit>();

			BufferLogic::Destroy(context,
				unit->vertexBuffer);

			BufferLogic::Destroy(context,
				unit->indexBuffer);

		}
		renderUnitEOs.clear();
	}


}