
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
        auto &cacheEO = context->renderCacheEo;

        auto instanceCache = std::make_shared<MaterialInstanceCache>();
        cacheEO->AddComponent(instanceCache);
    }

    void MaterialInstanceLogic::DestroyCache(Context *context)
    {
        auto &cacheEO = context->renderCacheEo;
        auto instanceCache = cacheEO->GetComponent<MaterialInstanceCache>();

        auto &sharedMap = instanceCache->sharedMap;
        for (auto &kv : sharedMap)
        {
            auto &instances = kv.second;
            for (auto &instance : instances)
            {
                Destroy(context, instance);
            }
        }
        sharedMap.clear();

        // TODO 没用的应该及时删除
        auto &deletes = instanceCache->deletes;
        for (auto &instance : deletes)
        {
            Destroy(context, instance);
        }
        deletes.clear();
    }

    std::shared_ptr<MaterialInstance> MaterialInstanceLogic::Get(Context *context,
                                                                 const std::string &pipelineName,
                                                                 const std::vector<std::string> &imageNames, bool isCube)
    {
        auto &cacheEO = context->renderCacheEo;
        auto instanceCache = cacheEO->GetComponent<MaterialInstanceCache>();

        auto &sharedMap = instanceCache->sharedMap;
        if (sharedMap.find(pipelineName) != sharedMap.end())
        {
            auto instances = sharedMap[pipelineName];
            for (auto &instance : instances)
            {
                auto equal = std::equal(imageNames.begin(), imageNames.end(), instance->imageNames.begin(),
                                        [](const std::string &str1, const std::string &str2)
                                        {
                                            return str1 == str2;
                                        });
                if (equal)
                {
                    return instance;
                }
            }
        }

        auto newInstance = Create(context, pipelineName, imageNames, isCube);
        sharedMap[pipelineName].push_back(newInstance);

        return newInstance;
    }

    std::shared_ptr<MaterialInstance> MaterialInstanceLogic::Create(Context *context,
                                                                    const std::string &pipelineName,
                                                                    const std::vector<std::string> &imageNames, bool isCube)
    {
        auto instance = std::make_shared<MaterialInstance>();
        instance->pipelineName = pipelineName;
        instance->imageNames = imageNames;

        if (isCube)
        {
            CreateImageCube(context, instance, imageNames);
        }
        else
        {
            for (auto &imageName : imageNames)
            {
                CreateImage(context, instance, imageName);
            }
        }

        CreateDescriptor(context, instance);

        return instance;
    }

    void MaterialInstanceLogic::Destroy(Context *context,
                                        std::shared_ptr<MaterialInstance> instance)
    {
        for (auto &image : instance->images)
        {
            ImageLogic::Destroy(context, image);
        }
        MaterialDescriptorLogic::Destroy(context, instance);
    }

    void MaterialInstanceLogic::SetDestroy(Context *context,
                                           std::shared_ptr<MaterialInstance> instance)
    {
        auto &cacheEO = context->renderCacheEo;
        auto instanceCache = cacheEO->GetComponent<MaterialInstanceCache>();

        instanceCache->deletes.push_back(instance);
    }

    void MaterialInstanceLogic::CreateImage(Context *context,
                                            std::shared_ptr<MaterialInstance> instance,
                                            const std::string &imageName)
    {
        auto &resImg = Common::ResSystem::LoadImg(imageName);

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

        instance->images = {imageCube};
    }

    void MaterialInstanceLogic::CreateDescriptor(Context *context,
                                                 std::shared_ptr<MaterialInstance> instance)
    {
        MaterialDescriptorLogic::AllocateAndUpdate(context, instance);
    }

}