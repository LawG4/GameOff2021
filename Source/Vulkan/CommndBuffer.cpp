/*!********************************************************************************************************
\File          : CommandBuffer.cpp
\Copyright     : GPL-3.0 License
\Brief         : Created command buffers and pools
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Memory.h"
#include "Vulkan.h"

std::vector<VkCommandPool> vk::graphicsPools;
std::vector<VkCommandBuffer> vk::cmdBuffers;

bool vk::allocateCommandBuffers()
{
    vk::cmdBuffers.clear();
    vk::cmdBuffers.resize(swapLength);

    for (uint32_t i = 0; i < vk::swapLength; i++) {
        VkCommandBufferAllocateInfo buffer;
        memset(&buffer, 0, sizeof(VkCommandBufferAllocateInfo));
        buffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        buffer.commandPool = vk::graphicsPools.at(i);
        buffer.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        buffer.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(vk::logicalDevice, &buffer, &vk::cmdBuffers.at(i)) != VK_SUCCESS) {
            Log.error("Failed to allocated graphics command buffers");
            return false;
        }
    }

    return true;
}

bool vk::createCommandPools()
{
    VkCommandPoolCreateInfo pool;
    memset(&pool, 0, sizeof(VkCommandPoolCreateInfo));

    pool.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool.queueFamilyIndex = vk::selectedQueueFamilies.graphicsFamily.value();

    graphicsPools.resize(vk::swapLength);
    for (uint32_t i = 0; i < vk::swapLength; i++) {
        if (vkCreateCommandPool(vk::logicalDevice, &pool, nullptr, &vk::graphicsPools.at(i)) != VK_SUCCESS) {
            Log.error("Couldn't create command pool");
            return false;
        }
    }

    return true;
}
