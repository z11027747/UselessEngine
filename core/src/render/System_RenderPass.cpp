
#include "render/comp.h"
#include "context.h"

//在进行管线创建之前，我们还需要设置用于渲染的帧缓冲附着。
//	我们需要指定使用的颜色和深度缓冲，以及采样数，渲染操作如何处理缓冲的内容

//RenderPass 渲染流程
void RenderSystem::CreateRenderPass(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapChainExtent = globalInfoComp->swapChainExtent;
	auto swapChainImageFormat = globalInfoComp->swapChainImageFormat;

	//交换链图像的颜色缓冲附着
	VkAttachmentDescription colorAttachmentDescription = {};
	colorAttachmentDescription.format = swapChainImageFormat;
	colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;

	//VkAttachmentLoadOp 渲染之前对附着中的数据进行的操作
	//	VK_ATTACHMENT_LOAD_OP_LOAD：保持附着的现有内容
	//	VK_ATTACHMENT_LOAD_OP_CLEAR：使用一个常量值来清除附着的内容
	//	VK_ATTACHMENT_LOAD_OP_DONT_CARE：不关心附着现存的内容
	colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

	//VkAttachmentStoreOp 渲染之后对附着中的数据进行的操作
	//	VK_ATTACHMENT_STORE_OP_STORE：渲染的内容会被存储起来，以便之后读取
	//	VK_ATTACHMENT_STORE_OP_DONT_CARE：渲染后，不会读取帧缓冲的内容
	colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	//VkImageLayout 图形内存布局
	//	VK_IMAGE_LAYOUT_UNDEFINED：不关心之前的图像布局方式
	//	VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL：图像被用作颜色附着
	//	VK_IMAGE_LAYOUT_PRESENT_SRC_KHR：图像被用在交换链中进行呈现操作
	//	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL：图像被用作复制操作的目的图像

	//指定渲染流程开始前的图像布局方式
	colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//指定渲染流程结束后的图像布局方式
	colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	//子流程
	//	一个渲染流程可以包含多个子流程，子流程依赖于上一流程处理后的帧缓冲内容
	VkSubpassDescription subpassDescription = {};

	//VkPipelineBindPoint 管线类型
	//	VK_PIPELINE_BIND_POINT_GRAPHICS：图形管线
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	//颜色附着引用
	VkAttachmentReference colorAttachmentReference = {};
	colorAttachmentReference.attachment = 0; //index
	colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//这里设置的颜色附着在数组中的索引会被片段着色器使用
	//	对应我们在片段着色器中使用的 layout(location = 0) out vec4 outColor语句
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorAttachmentReference;

	VkRenderPassCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.attachmentCount = 1;
	createInfo.pAttachments = &colorAttachmentDescription;
	createInfo.subpassCount = 1;
	createInfo.pSubpasses = &subpassDescription;

	//子流程依赖：描述每一步的subpass什么时候可以进行下一步的subpass

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL; //开始前的子流程，代表隐含的子流程
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; //等待交换链结束对图像的读取
	dependency.srcAccessMask = 0;

	dependency.dstSubpass = 0; //结束后的子流程，是之前创建的subass的索引0
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
		| VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; //目标子pass需要读取和写入颜色附件

	createInfo.dependencyCount = 1;
	createInfo.pDependencies = &dependency;

	auto ret = vkCreateRenderPass(logicDevice, &createInfo, nullptr, &globalInfoComp->renderPass);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("failed createRenderPass error!");
	}

}

void RenderSystem::DestroyRenderPass(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& renderPass = globalInfoComp->renderPass;

	vkDestroyRenderPass(logicDevice, renderPass, nullptr);
}
