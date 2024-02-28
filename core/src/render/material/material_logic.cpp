
#include "render/material/material_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "common/res_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialLogic::TryDestroyEO(Context *context, std::shared_ptr<EngineObject> eo)
    {
        auto material = eo->GetComponent<Render::Material>();
        MaterialLogic::SetDestroy(context, material->data);
    }

    static int Id_Material = 0;

    std::shared_ptr<MaterialData> MaterialLogic::Create(Context *context,
                                                        const std::string &pipelineName)
    {
        auto info = std::make_shared<MaterialInfo>();
        info->pipelineName = pipelineName;
        return Create(context, info);
    }
    std::shared_ptr<MaterialData> MaterialLogic::Create(Context *context,
                                                        std::shared_ptr<MaterialInfo> info)
    {
        auto data = std::make_shared<MaterialData>();
        data->id = Id_Material++;
        data->info = info;

        if (info->isImageCube)
        {
            GetOrCreateImageCube(context, data, info->imageNames);
        }
        else
        {
            for (auto &imageName : info->imageNames)
            {
                GetOrCreateImage(context, data, imageName);
            }
        }

        CreateBuffer(context, data);
        CreateDescriptor(context, data);

        return data;
    }

    void MaterialLogic::Destroy(Context *context,
                                std::shared_ptr<MaterialData> data)
    {
        BufferLogic::Destroy(context, data->buffer);

        if (data->descriptor != nullptr)
            MaterialDescriptorLogic::Destroy(context, data);
    }

    void MaterialLogic::SetDestroy(Context *context,
                                   std::shared_ptr<MaterialData> data)
    {
        auto &globalEO = context->renderGlobalEO;
        auto materialCache = globalEO->GetComponent<MaterialCache>();

        materialCache->deletes.push_back(data);
    }

    void MaterialLogic::GetOrCreateImage(Context *context,
                                         std::shared_ptr<MaterialData> data,
                                         const std::string &imageName)
    {
        auto &globalEO = context->renderGlobalEO;
        auto materialCache = globalEO->GetComponent<MaterialCache>();

        auto &sharedImageMap = materialCache->sharedImageMap;
        if (sharedImageMap.find(imageName) != sharedImageMap.end())
        {
            auto sharedImage = sharedImageMap[imageName];
            data->images.push_back(sharedImage);
            return;
        }

        CreateImage(context,
                    data, imageName);

        sharedImageMap[imageName] = data->images.back();
    }

    void MaterialLogic::GetOrCreateImageCube(Context *context,
                                             std::shared_ptr<MaterialData> data,
                                             const std::vector<std::string> &imageNames)
    {
        auto &globalEO = context->renderGlobalEO;
        auto materialCache = globalEO->GetComponent<MaterialCache>();

        auto &sharedImageMap = materialCache->sharedImageMap;
        if (sharedImageMap.find(imageNames[0]) != sharedImageMap.end())
        {
            auto sharedImage = sharedImageMap[imageNames[0]];
            data->images = {sharedImage};
            return;
        }

        CreateImageCube(context,
                        data,
                        imageNames);

        sharedImageMap[imageNames[0]] = data->images.back();
    }

    void MaterialLogic::CreateImage(Context *context,
                                    std::shared_ptr<MaterialData> data,
                                    const std::string &imageName)
    {
        auto &resImg = Common::ResSystem::LoadImg(imageName);

        auto imageW = static_cast<uint32_t>(resImg.width);
        auto imageH = static_cast<uint32_t>(resImg.height);
        auto imageSize = static_cast<VkDeviceSize>(imageW * imageH * 4);

        // max(w,h), log2, floor+1
        auto mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(resImg.width, resImg.height)))) + 1;

        auto tempBuffer = BufferLogic::CreateTemp(context,
                                                  imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        BufferSetLogic::SetPtr(context,
                               tempBuffer,
                               resImg.data, static_cast<size_t>(imageSize));

        ImageCreateInfo imageCreateInfo = {
            VK_FORMAT_R8G8B8A8_UNORM, {imageW, imageH, 1}, VK_IMAGE_ASPECT_COLOR_BIT,
            // info
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            0,
            1,
            VK_IMAGE_VIEW_TYPE_2D,
            mipLevels,
            VK_SAMPLE_COUNT_1_BIT,
            // memory
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            // layout
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL};
        auto image2d = ImageLogic::CreateByInfo(context, imageCreateInfo);

        ImageLogic::CopyFromBuffer(context,
                                   image2d,
                                   tempBuffer);

        ImageLogic::GenerateMipmapsAndTransitionLayout(context, image2d,
                                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        data->images.push_back(image2d);
    }

    void MaterialLogic::CreateImageCube(Context *context,
                                        std::shared_ptr<MaterialData> data,
                                        const std::vector<std::string> &imageNames)
    {
        uint32_t imageCubeW, imageCubeH = 0;
        std::vector<unsigned char *> datas(6);

        for (auto i = 0; i < 6; i++)
        {
            auto &resImg = Common::ResSystem::LoadImg(imageNames[i]);

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

        ImageCreateInfo imageCreateInfo = {
            VK_FORMAT_R8G8B8A8_UNORM, {imageCubeW, imageCubeH, 1}, VK_IMAGE_ASPECT_COLOR_BIT,
            // info
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
            6,
            VK_IMAGE_VIEW_TYPE_CUBE,
            1,
            VK_SAMPLE_COUNT_1_BIT,
            // memory
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            // layout
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL};
        auto imageCube = ImageLogic::CreateByInfo(context, imageCreateInfo);

        ImageLogic::CopyFromBuffer(context,
                                   imageCube,
                                   tempBuffer);

        ImageLogic::TransitionLayout(context,
                                     imageCube,
                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);

        data->images = {imageCube};
    }

    void MaterialLogic::CreateBuffer(Context *context,
                                     std::shared_ptr<MaterialData> data)
    {
        auto uboSize = sizeof(MaterialUBO);
        CreateBuffer(context, data, uboSize);
    }
    void MaterialLogic::CreateBuffer(Context *context,
                                     std::shared_ptr<MaterialData> data, size_t uboSize)
    {
        auto buffer = BufferLogic::Create(context,
                                          uboSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        data->buffer = buffer;
    }

    void MaterialLogic::CreateDescriptor(Context *context,
                                         std::shared_ptr<MaterialData> data)
    {
        MaterialDescriptorLogic::AllocateAndUpdate(context, data);
    }

}