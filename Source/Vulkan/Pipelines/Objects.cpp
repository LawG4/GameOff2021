
#include "Objects.h"
#include "Vulkan.h"

RenderObject::RenderObject()
{
    // Require a UBO update
    requiresUBOUpdateVector.resize(vk::swapLength, true);

    // Set is active to trie
    isActive = true;

    // Memset our internal vulkan structs
    memset(&vertexGroup, 0, sizeof(vk::BufferGroup));
    memset(&indexGroup, 0, sizeof(vk::BufferGroup));
}

RenderObject::~RenderObject()
{
    for (auto& ubo : ubos) {
        if (ubo.buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(vk::logialDevice, ubo.buffer, nullptr);
            vkFreeMemory(vk::logialDevice, ubo.mem, nullptr);
        }
    }

    // Free the vertex buffer
    if (vertexGroup.buffer != VK_NULL_HANDLE) vkDestroyBuffer(vk::logialDevice, vertexGroup.buffer, nullptr);
    if (vertexGroup.mem != VK_NULL_HANDLE) vkFreeMemory(vk::logialDevice, vertexGroup.mem, nullptr);

    vkDestroyDescriptorPool(vk::logialDevice, pool, nullptr);
}

bool RenderObject::requiresUBOUpdate(uint32_t swapIndex) { return requiresUBOUpdateVector[swapIndex]; }

void RenderObject::scheduleUBOUpdate()
{
    requiresUBOUpdateVector.clear();
    requiresUBOUpdateVector.resize(vk::swapLength, true);
}
