/*!********************************************************************************************************
\File          : Buffer.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides utility for writing useful bufers
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Memory.h"

VkPhysicalDeviceMemoryProperties vk::memory::memProperties;
std::vector<vk::BufferGroup> vk::bufferStorageMap;

/// <summary> Physical devices provide different types of memory, find the index of the type of memory which
/// meets the requirements for the buffer we want to allocate </summary> <param name="typeFilter"></param>
/// <param name="typeFilter">A bit mask of indexes which might be suitable(?)</param>
/// <param name="properties">A bit mask of properties that we require the memory to have</param>
/// <returns>The index of device memory which satisfies our needs</returns>
uint32_t findMemoryIndex(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    // Loop through each type of memory supplied by the device
    for (uint32_t i = 0; i < vk::memory::memProperties.memoryTypeCount; i++) {
        if ((typeFilter & 1 << i) && /*If type is appropariate*/
            (vk::memory::memProperties.memoryTypes[i].propertyFlags & properties) ==
              properties /*If this memory type provides all the flags required by properties*/) {
            return i;
        }
    }

    // Couldn't find a suitable device memory index
    Log.error("Couldn't find a suitable device memory");
    return static_cast<uint32_t>(-1);  // Just fuck things up not exactly safe but oh well
}

vk::BufferGroup vk::createBufferGroup(VkDeviceSize size, VkBufferUsageFlags usage,
                                      VkMemoryPropertyFlags properties)
{
    vk::BufferGroup buff;
    memset(&buff, 0, sizeof(vk::BufferGroup));

    // Create the buffer
    VkBufferCreateInfo buffer;
    memset(&buffer, 0, sizeof(VkBufferCreateInfo));
    buffer.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer.size = size;
    buffer.usage = usage;
    buffer.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(vk::logialDevice, &buffer, nullptr, &buff.buffer) != VK_SUCCESS) {
        Log.error("Could not create vkBuffer");
        return buff;
    }

    // Get the memory properties of this buffer so we know what kind of memory we need to allocate
    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(vk::logialDevice, buff.buffer, &requirements);

    // Allocate the underlying memory
    VkMemoryAllocateInfo alloc;
    memset(&alloc, 0, sizeof(VkMemoryAllocateInfo));
    alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc.allocationSize = requirements.size;
    alloc.memoryTypeIndex = findMemoryIndex(requirements.memoryTypeBits, properties);

    if (vkAllocateMemory(vk::logialDevice, &alloc, nullptr, &buff.mem) != VK_SUCCESS) {
        Log.error("Could not allocate vkMemory");
        return buff;
    }

    // Associate the buffer with its device memory
    vkBindBufferMemory(vk::logialDevice, buff.buffer, buff.mem, 0);
    return buff;
}

void copyBufferWithRecordingCmdBuffer(const VkBuffer& src, const VkBuffer& dst, VkDeviceSize size,
                                      VkCommandBuffer& cmd)
{
    // Assumes command buffer is currently recording already
    VkBufferCopy copy;
    memset(&copy, 0, sizeof(VkBufferCopy));
    copy.size = size;

    vkCmdCopyBuffer(cmd, src, dst, 1, &copy);
}

void copyBufferWithFreshCmdBuffer(const VkBuffer& src, const VkBuffer& dst, VkDeviceSize size)
{
    // Allocate a fresh command buffer from the graphics pool as it is guarenteed tp support transfer
    VkCommandBufferAllocateInfo alloc;
    memset(&alloc, 0, sizeof(VkCommandBufferAllocateInfo));
    alloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc.commandBufferCount = 1;
    alloc.commandPool = vk::graphicsPools.at(0);
    alloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer cmd;
    vkAllocateCommandBuffers(vk::logialDevice, &alloc, &cmd);

    // Start recording the command buffer
    VkCommandBufferBeginInfo begin;
    memset(&begin, 0, sizeof(VkCommandBufferBeginInfo));
    begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &begin);

    // Copy the buffers
    copyBufferWithRecordingCmdBuffer(src, dst, size, cmd);

    // end command buffer
    vkEndCommandBuffer(cmd);

    // SubGPL-3.0 Licensethe command buffer
    VkSubmitInfo submit;
    memset(&submit, 0, sizeof(VkSubmitInfo));
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmd;

    vkQueueSubmit(vk::graphicsQueue, 1, &submit, VK_NULL_HANDLE);
    vkQueueWaitIdle(vk::graphicsQueue);

    // Free the command buffer
    vkFreeCommandBuffers(vk::logialDevice, vk::graphicsPools.at(0), 1, &cmd);
}

void vk::addVertexBuffer(const char* bufferName, const std::vector<Vertex>& vertexBuffer)
{
    // Get the size of the memory we need
    VkDeviceSize size = sizeof(Vertex) * vertexBuffer.size();

    // Create a vertex buffer that is not visible to the host so that it's faster to access, it also needs to
    // be able to accept transfers
    vk::BufferGroup nonVisible =
      createBufferGroup(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    // Create the staging buffer that is CPU visible and can be used to send the transfer to our device local
    // vertex buffer
    vk::BufferGroup buff =
      createBufferGroup(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // Now we write the contents of the vertices to the staging buffer
    // Start by mapping the memory on the GPU to a CPU visible section of memory
    void* data;
    vkMapMemory(vk::logialDevice, buff.mem, 0, sizeof(Vertex) * vertexBuffer.size(), 0, &data);

    // mem copy the content of our vertex buffer into the mapped memory
    memcpy(data, vertexBuffer.data(), sizeof(Vertex) * vertexBuffer.size());

    // Free the device memory from the CPU
    vkUnmapMemory(vk::logialDevice, buff.mem);

    // Copy the contents of the staging buffer over to the vertex buffer
    copyBufferWithFreshCmdBuffer(buff.buffer, nonVisible.buffer, size);

    // Free the staging buffer
    vkDestroyBuffer(vk::logialDevice, buff.buffer, nullptr);
    vkFreeMemory(vk::logialDevice, buff.mem, nullptr);

    // Finally add this to the internal buffer register
    bufferStorageMap.push_back(nonVisible);
}

void vk::destroyBuffers()
{
    for (const vk::BufferGroup& buff : bufferStorageMap) {
        if (buff.buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(vk::logialDevice, buff.buffer, nullptr);
        }

        if (buff.mem != VK_NULL_HANDLE) {
            vkFreeMemory(vk::logialDevice, buff.mem, nullptr);
        }
    }
}
