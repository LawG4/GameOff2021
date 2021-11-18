
#include "Memory.h"
#include "Sprites.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// All the sprite sheets share one command pool, so they can be reset independantly from the per frame buffers
VkCommandPool SpriteInternals::sheetPool = VK_NULL_HANDLE;

SpriteSheet::SpriteSheet(const char* TextureFileName)
{
    // Has the command pool been allocated
    if (SpriteInternals::sheetPool == VK_NULL_HANDLE) {
        VkCommandPoolCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        info.queueFamilyIndex = vk::selectedQueueFamilies.graphicsFamily.value();

        if (vkCreateCommandPool(vk::logicalDevice, &info, nullptr, &SpriteInternals::sheetPool) !=
            VK_SUCCESS) {
            Log.error("Could not allocate the command pool");
            return;
        }
    }

    // First upload the texture to the GPU
    int width, height, channels;
    stbi_uc* pixels = stbi_load(TextureFileName, &width, &height, &channels, STBI_rgb_alpha);
    VkDeviceSize size = width * height * 4;  // $ texture channels

    if (!pixels) {
        Log.error("Could not load image {}", TextureFileName);
        return;
    }

    // We're going to take this raw pixel data and upload it into a buffer and then copy it to an image
    vk::BufferGroup stagingGroup =
      vk::createBufferGroup(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* data;
    vkMapMemory(vk::logicalDevice, stagingGroup.mem, 0, size, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(size));
    vkUnmapMemory(vk::logicalDevice, stagingGroup.mem);

    // Free the pixels on the CPU
    stbi_image_free(pixels);

    // Create the Vulkan Image handle for this struct
    {
        VkImageCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        info.imageType = VK_IMAGE_TYPE_2D;
        info.extent.width = static_cast<uint32_t>(width);
        info.extent.height = static_cast<uint32_t>(height);
        info.extent.depth = 1;
        info.mipLevels = 1;
        info.arrayLayers = 1;
        info.format = VK_FORMAT_R8G8B8A8_SRGB;
        info.tiling = VK_IMAGE_TILING_OPTIMAL;
        info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        info.samples = VK_SAMPLE_COUNT_1_BIT;

        if (vkCreateImage(vk::logicalDevice, &info, nullptr, &texture) != VK_SUCCESS) {
            Log.error("Could not create image for texture");
            return;
        }
    }

    // Allocate device memory underlying this image
    {
        VkMemoryRequirements requirements;
        vkGetImageMemoryRequirements(vk::logicalDevice, texture, &requirements);

        VkMemoryAllocateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        info.allocationSize = requirements.size;
        info.memoryTypeIndex =
          vk::findMemoryIndex(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (vkAllocateMemory(vk::logicalDevice, &info, nullptr, &textureMemory) != VK_SUCCESS) {
            Log.error("Could not allocate memory for the image");
            return;
        }

        // Assocate th image to it's underlying memory
        vkBindImageMemory(vk::logicalDevice, texture, textureMemory, 0);
    }

    // Create and record a brand new command buffer to upload the image
    {
        VkCommandBuffer cmd;

        {
            // allocate the command buffer
            VkCommandBufferAllocateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            info.commandBufferCount = 1;
            info.commandPool = vk::graphicsPools[0];
            info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            vkAllocateCommandBuffers(vk::logicalDevice, &info, &cmd);

            // Start recording
            VkCommandBufferBeginInfo begin{};
            begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            vkBeginCommandBuffer(cmd, &begin);
        }

        // use a pipeline barrier to transition the image to transfer_dst so it can be copied to
        // We'll reuse this create info later
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barrier.image = texture;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
                             nullptr, 0, nullptr, 1, &barrier);

        {
            // Now perform the copy from the buffer to the image
            VkBufferImageCopy info{};
            info.bufferOffset = 0;
            info.bufferRowLength = 0;
            info.bufferImageHeight = 0;

            info.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            info.imageSubresource.mipLevel = 0;
            info.imageSubresource.baseArrayLayer = 0;
            info.imageSubresource.layerCount = 1;

            info.imageOffset = {0, 0, 0};
            info.imageExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};

            vkCmdCopyBufferToImage(cmd, stagingGroup.buffer, texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
                                   &info);
        }

        // Do another barrier to transition the image from transfer_dst into Shader_read_only
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
                             nullptr, 0, nullptr, 1, &barrier);

        // End and submit the commandbuffer
        {
            vkEndCommandBuffer(cmd);
            VkSubmitInfo info{};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.commandBufferCount = 1;
            info.pCommandBuffers = &cmd;

            vkQueueSubmit(vk::graphicsQueue, 1, &info, 0);

            vkQueueWaitIdle(vk::graphicsQueue);
        }
    }

    // Free the staging buffer
    vkDestroyBuffer(vk::logicalDevice, stagingGroup.buffer, nullptr);
    vkFreeMemory(vk::logicalDevice, stagingGroup.mem, nullptr);

    // Create an image view for this texture image
    {
        VkImageViewCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.image = texture;
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.format = VK_FORMAT_R8G8B8A8_SRGB;
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(vk::logicalDevice, &info, nullptr, &textureView) != VK_SUCCESS) {
            Log.error("Could not create image view");
            return;
        }
    }

    // Create a sampler for this object
    {
        VkSamplerCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        info.magFilter = VK_FILTER_LINEAR;
        info.minFilter = VK_FILTER_LINEAR;
        info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        info.anisotropyEnable = VK_FALSE;
        info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        info.unnormalizedCoordinates = VK_FALSE;
        info.compareEnable = VK_FALSE;

        info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        info.mipLodBias = 0;
        info.minLod = 0;
        info.maxLod = 0;

        if (vkCreateSampler(vk::logicalDevice, &info, nullptr, &sampler) != VK_SUCCESS) {
            Log.error("Could not create samplers");
            return;
        }
    }

    // Create the desciptor pool
    {
        VkDescriptorPoolCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        info.maxSets = 1 * vk::swapLength;

        VkDescriptorPoolSize size{};
        size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        size.descriptorCount = 1 * vk::swapLength;
        info.poolSizeCount = 1;
        info.pPoolSizes = &size;

        if (vkCreateDescriptorPool(vk::logicalDevice, &info, nullptr, &pool) != VK_SUCCESS) {
            Log.error("Could not create descriptor pools");
            return;
        }
    }

    // Create descriptor set layout
    {
        VkDescriptorSetLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        info.bindingCount = 1;

        VkDescriptorSetLayoutBinding binding{};
        binding.binding = 0;
        binding.descriptorCount = 1;
        binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        info.bindingCount = 1;
        info.pBindings = &binding;

        vkCreateDescriptorSetLayout(vk::logicalDevice, &info, nullptr, &layout);

        VkDescriptorSetAllocateInfo alloc{};
        std::vector<VkDescriptorSetLayout> layouts(vk::swapLength, layout);
        sets.resize(vk::swapLength);
        alloc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc.descriptorPool = pool;
        alloc.descriptorSetCount = vk::swapLength;
        alloc.pSetLayouts = layouts.data();
        vkAllocateDescriptorSets(vk::logicalDevice, &alloc, sets.data());
    }

    // Update the descriptor sets so that it points to the image
    for (uint32_t i = 0; i < vk::swapLength; i++) {
        VkWriteDescriptorSet writer{};
        writer.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writer.descriptorCount = 1;
        writer.dstBinding = 0;
        writer.dstSet = sets.at(i);
        writer.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

        VkDescriptorImageInfo image{};
        image.sampler = sampler;
        image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        image.imageView = textureView;
        writer.pImageInfo = &image;

        vkUpdateDescriptorSets(vk::logicalDevice, 1, &writer, 0, nullptr);
    }
}

void SpriteSheet::addSprite(Sprite* sprite) { sprites.push_back(sprite); }

void SpriteSheet::appendCommands(VkCommandBuffer& cmd, const VkPipelineLayout& layout)
{
    // Bind to the texture descriptor set for this sprite sheet
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &sets.at(0), 0, nullptr);

    // Loop through each sprite that belongs to this sprite sheet
    for (Sprite* sprite : sprites) {
        sprite->appendCommands(cmd, layout);
    }
}

SpriteSheet::~SpriteSheet()
{
    // Wait for the device in use to finish
    vkDeviceWaitIdle(vk::logicalDevice);

    if (textureView != VK_NULL_HANDLE) {
        vkDestroyImageView(vk::logicalDevice, textureView, nullptr);
    }

    if (texture != VK_NULL_HANDLE) {
        vkDestroyImage(vk::logicalDevice, texture, nullptr);
    }

    if (textureMemory != VK_NULL_HANDLE) {
        vkFreeMemory(vk::logicalDevice, textureMemory, nullptr);
    }

    if (sampler != VK_NULL_HANDLE) {
        vkDestroySampler(vk::logicalDevice, sampler, nullptr);
    }

    if (layout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(vk::logicalDevice, layout, nullptr);
    }

    if (pool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(vk::logicalDevice, pool, nullptr);
    }

    if (SpriteInternals::sheetPool != VK_NULL_HANDLE) {
        vkDestroyCommandPool(vk::logicalDevice, SpriteInternals::sheetPool, nullptr);
    }
}
