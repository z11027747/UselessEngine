
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
    void MaterialInstanceLogic::CreateCache(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;

        auto instanceCache = std::make_shared<MaterialInstanceCache>();
        globalEO->AddComponent(instanceCache);
    }

    void MaterialInstanceLogic::DestroyCache(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto instanceCache = globalEO->GetComponent<MaterialInstanceCache>();

        auto &sharedImageMap = instanceCache->sharedImageMap;
        for (auto &kv : sharedImageMap)
        {
            auto &sharedImage = kv.second;
            ImageLogic::Destroy(context, sharedImage);
        }

        // TODO 没用的应该及时删除
        auto &deletes = instanceCache->deletes;
        for (auto &instance : deletes)
        {
            Destroy(context, instance);
        }
        deletes.clear();

        globalEO->RemoveComponent<MaterialInstanceCache>();
    }

    static int Id_Material = 0;

    std::shared_ptr<MaterialInstance> MaterialInstanceLogic::Create(Context *context,
                                                                    std::shared_ptr<MaterialInfo> info)
    {
        auto instance = std::make_shared<MaterialInstance>();
        instance->id = Id_Material++;
        instance->info = info;

        if (info->isImageCube)
        {
            GetOrCreateImageCube(context, instance, info->imageNames);
        }
        else
        {
            for (auto &imageName : info->imageNames)
            {
                GetOrCreateImage(context, instance, imageName);
            }
        }

        CreateBuffer(context, instance);

        CreateDescriptor(context, instance);

        return instance;
    }

    void MaterialInstanceLogic::Destroy(Context *context,
                                        std::shared_ptr<MaterialInstance> instance)
    {
        BufferLogic::Destroy(context, instance->buffer);
        // Common::LogSystem::Info("DestroyBuffer: ", instance->id);

        if (instance->descriptor != nullptr)
            MaterialDescriptorLogic::Destroy(context, instance);
    }

    void MaterialInstanceLogic::SetDestroy(Context *context,
                                           std::shared_ptr<MaterialInstance> instance)
    {
        auto &globalEO = context->renderGlobalEO;
        auto instanceCache = globalEO->GetComponent<MaterialInstanceCache>();

        instanceCache->deletes.push_back(instance);
    }

    void MaterialInstanceLogic::GetOrCreateImage(Context *context,
                                                 std::shared_ptr<MaterialInstance> instance,
                                                 const std::string &imageName)
    {
        auto &globalEO = context->renderGlobalEO;
        auto instanceCache = globalEO->GetComponent<MaterialInstanceCache>();

        auto &sharedImageMap = instanceCache->sharedImageMap;
        if (sharedImageMap.find(imageName) != sharedImageMap.end())
        {
            auto sharedImage = sharedImageMap[imageName];
            instance->images.push_back(sharedImage);
            return;
        }

        CreateImage(context,
                    instance, imageName);

        sharedImageMap[imageName] = instance->images.back();
    }

    void MaterialInstanceLogic::GetOrCreateImageCube(Context *context,
                                                     std::shared_ptr<MaterialInstance> instance,
                                                     const std::vector<std::string> &imageNames)
    {
        auto &globalEO = context->renderGlobalEO;
        auto instanceCache = globalEO->GetComponent<MaterialInstanceCache>();

        auto &sharedImageMap = instanceCache->sharedImageMap;
        if (sharedImageMap.find(imageNames[0]) != sharedImageMap.end())
        {
            auto sharedImage = sharedImageMap[imageNames[0]];
            instance->images = {sharedImage};
            return;
        }

        CreateImageCube(context,
                        instance,
                        imageNames);

        sharedImageMap[imageNames[0]] = instance->images.back();
    }

    void MaterialInstanceLogic::CreateImage(Context *context,
                                            std::shared_ptr<MaterialInstance> instance,
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

        instance->images.push_back(image2d);
    }

    void MaterialInstanceLogic::CreateImageCube(Context *context,
                                                std::shared_ptr<MaterialInstance> instance,
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

        instance->images = {imageCube};
    }

    void MaterialInstanceLogic::CreateBuffer(Context *context,
                                             std::shared_ptr<MaterialInstance> instance)
    {
        auto uboSize = sizeof(MaterialUBO);
        auto buffer = BufferLogic::Create(context,
                                          uboSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        instance->buffer = buffer;
        // Common::LogSystem::Info("CreateBuffer: ", instance->id);
    }

    void MaterialInstanceLogic::CreateDescriptor(Context *context,
                                                 std::shared_ptr<MaterialInstance> instance)
    {
        MaterialDescriptorLogic::AllocateAndUpdate(context, instance);
    }

}