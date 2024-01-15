
#include "render/material/material_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "common/res_system.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{

    void MaterialLogic::Create(Context *context,
                               std::shared_ptr<Material> material)
    {
        if (material->image0Names.size() == 1)
            CreateImage(context, material);
        else if (material->image0Names.size() == 6)
            CreateImageCube(context, material);

        CreateDescriptor(context, material);
    }

    void MaterialLogic::Destroy(Context *context,
                                std::shared_ptr<Material> material)
    {
        ImageLogic::Destroy(context, material->image0);
        MaterialDescriptorLogic::Destroy(context, material);
    }

    void MaterialLogic::CreateImage(Context *context,
                                    std::shared_ptr<Material> material)
    {
        auto &resImg = Common::ResSystem::LoadImg(material->image0Names[0]);

        auto imageW = static_cast<uint32_t>(resImg.width);
        auto imageH = static_cast<uint32_t>(resImg.height);
        auto imageSize = static_cast<VkDeviceSize>(imageW * imageH * 4);

        auto tempBuffer = BufferLogic::CreateTemp(context,
                                                  imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        BufferSetLogic::SetPtr(context,
                               tempBuffer,
                               resImg.data, static_cast<size_t>(imageSize));

        ImageInfo image2dInfo = {
            VK_FORMAT_R8G8B8A8_UNORM, {imageW, imageH, 1}, VK_IMAGE_ASPECT_COLOR_BIT,
            // image
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            0,
            1,
            VK_IMAGE_VIEW_TYPE_2D,
            // memory
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            // layout
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL};
        auto image2d = ImageLogic::CreateByInfo(context, image2dInfo);

        ImageLogic::CopyFromBuffer(context,
                                   image2d,
                                   tempBuffer);

        ImageLogic::TransitionLayout(context,
                                     image2d,
                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        material->image0 = image2d;
    }

    void MaterialLogic::CreateImageCube(Context *context,
                                        std::shared_ptr<Material> material)
    {
        uint32_t imageCubeW, imageCubeH = 0;

        std::vector<unsigned char *> datas(6);

        for (auto i = 0; i < 6; i++)
        {
            auto &resImg = Common::ResSystem::LoadImg(material->image0Names[i]);

            imageCubeW = static_cast<uint32_t>(resImg.width);
            imageCubeH = static_cast<uint32_t>(resImg.height);

            datas[i] = resImg.data;
        }

        auto imageCubeSizeOne = static_cast<VkDeviceSize>(imageCubeW * imageCubeH * 4);
        auto imageCubeSizeTotal = imageCubeSizeOne * 6;

        auto tempBuffer = BufferLogic::CreateTemp(context,
                                                  imageCubeSizeTotal, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        BufferSetLogic::SetPtrVector(context,
                                     tempBuffer,
                                     datas, imageCubeSizeOne);

        ImageInfo imageCubeInfo = {
            VK_FORMAT_R8G8B8A8_UNORM, {imageCubeW, imageCubeH, 1}, VK_IMAGE_ASPECT_COLOR_BIT,
            // image
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
            6,
            VK_IMAGE_VIEW_TYPE_CUBE,
            // memory
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            // layout
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL};
        auto imageCube = ImageLogic::CreateByInfo(context, imageCubeInfo);

        ImageLogic::CopyFromBuffer(context,
                                   imageCube,
                                   tempBuffer);

        ImageLogic::TransitionLayout(context,
                                     imageCube,
                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        material->image0 = imageCube;
    }

    void MaterialLogic::CreateDescriptor(Context *context,
                                         std::shared_ptr<Material> material)
    {
        MaterialDescriptorLogic::AllocateAndUpdate(context, material);
    }

}