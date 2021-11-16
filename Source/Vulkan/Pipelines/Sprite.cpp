
#include "Sprites.h"
#include "Vulkan.h"

Sprite::Sprite()
{
    // Require a UBO update
    requiresUBOUpdateVector.resize(vk::swapLength, true);

    // Set is active to trie
    isActive = true;

    // Memset our internal vulkan structs
    memset(&vertexGroup, 0, sizeof(vk::BufferGroup));
    memset(&indexGroup, 0, sizeof(vk::BufferGroup));
}

Sprite::~Sprite()
{
    for (auto& ubo : ubos) {
        if (ubo.buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(vk::logicalDevice, ubo.buffer, nullptr);
            vkFreeMemory(vk::logicalDevice, ubo.mem, nullptr);
        }
    }

    // Free the vertex buffer
    if (vertexGroup.buffer != VK_NULL_HANDLE) vkDestroyBuffer(vk::logicalDevice, vertexGroup.buffer, nullptr);
    if (vertexGroup.mem != VK_NULL_HANDLE) vkFreeMemory(vk::logicalDevice, vertexGroup.mem, nullptr);

    vkDestroyDescriptorPool(vk::logicalDevice, pool, nullptr);
}

bool Sprite::requiresUBOUpdate(uint32_t swapIndex) { return requiresUBOUpdateVector[swapIndex]; }

void Sprite::scheduleUBOUpdate()
{
    requiresUBOUpdateVector.clear();
    requiresUBOUpdateVector.resize(vk::swapLength, true);
}
