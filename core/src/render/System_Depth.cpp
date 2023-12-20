
#include "render/comp.h"
#include "context.h"

//深度附着和颜色附着一样都是基于图像对象
//交换链不会自动地为我们创建深度附着使用的深度图像对象。我们需要自己创建深度图像对象
// 
//和纹理图像不同，深度图像不需要特定的图像数据格式
void RenderSystem::CreateDepth(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapChainExtent = globalInfoComp->swapChainExtent;

	//对于深度图像，只需要使用一个颜色通道
	const std::vector<VkFormat> candidates = {
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT
	};

	auto& depthFormat = globalInfoComp->depthFormat;
	depthFormat = FindSupportedFormat(context,
		candidates,
		VK_IMAGE_TILING_OPTIMAL,  //表示图像数据在内存中的优化排列方式
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT //对图像格式支持深度/模板附件操作的特性
	);

	auto& depthImage = globalInfoComp->depthImage;
	auto& depthImageMemory = globalInfoComp->depthImageMemory;
	auto& depthImageView = globalInfoComp->depthImageView;

	CreateImage(context,
		swapChainExtent.width, swapChainExtent.height, depthFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		//memory
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		//return
		depthImage, depthImageMemory);

	CreateImageView(context,
		depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT,
		depthImage, depthImageView);

	//渲染流程开始后首先会清除深度附着中的数据，不需要复制数据到深度图像中

	//需要对图像布局进行变换，来让它适合作为深度附着使用。
	TransitionImageLayout(context,
		depthImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}


void RenderSystem::DestroyDepth(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapChainExtent = globalInfoComp->swapChainExtent;

	auto& depthImage = globalInfoComp->depthImage;
	auto& depthImageMemory = globalInfoComp->depthImageMemory;
	auto& depthImageView = globalInfoComp->depthImageView;

	DestroyImage(context, depthImage, depthImageMemory);
	DestroyImageView(context, depthImageView);
}