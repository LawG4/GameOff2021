/*!********************************************************************************************************
\File          : CommandBuffer.cpp
\Copyright     : MIT License
\Brief         : Created command buffers and pools
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"

VkCommandPool vk::graphicsPool;
std::vector<VkCommandBuffer> vk::cmdBuffers;

bool vk::allocateCommandBuffers()
{
    VkCommandBufferAllocateInfo buffer;
    memset(&buffer, 0, sizeof(VkCommandBufferAllocateInfo));
    buffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    buffer.commandPool = vk::graphicsPool;
    buffer.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    buffer.commandBufferCount = vk::swapLength;

    vk::cmdBuffers.resize(swapLength);

    if (vkAllocateCommandBuffers(vk::logialDevice, &buffer, vk::cmdBuffers.data()) != VK_SUCCESS) {
        Log.error("Failed to allocated graphics command buffers");
        return false;
    }
    return true;
}

bool vk::createCommandPools()
{
    VkCommandPoolCreateInfo pool;
    memset(&pool, 0, sizeof(VkCommandPoolCreateInfo));

    pool.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool.queueFamilyIndex = vk::selectedQueueFamilies.graphicsFamily.value();

    if (vkCreateCommandPool(vk::logialDevice, &pool, nullptr, &vk::graphicsPool) != VK_SUCCESS) {
        Log.error("Couldn't create command pool");
        return false;
    }
    return true;
}
