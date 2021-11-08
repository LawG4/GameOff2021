/*!********************************************************************************************************
\File          : Buffer.cpp
\Copyright     : MIT License
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

vk::BufferGroup createBufferGroup(VkDeviceSize size, VkBufferUsageFlags usage,
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

void vk::addVertexBuffer(const char* bufferName, const std::vector<Vertex>& vertexBuffer)
{
    // Get the size of the memory we need
    VkDeviceSize size = sizeof(Vertex) * vertexBuffer.size();

    // Create the buffer group using our utility function
    vk::BufferGroup buff =
      createBufferGroup(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // Now we write the contents of the vertex buffer to the buffer on the device
    // Start by mapping the memory on the GPU to a CPU visible section of memory
    void* data;
    vkMapMemory(vk::logialDevice, buff.mem, 0, sizeof(Vertex) * vertexBuffer.size(), 0, &data);

    // mem copy the content of our vertex buffer into the mapped memory
    memcpy(data, vertexBuffer.data(), sizeof(Vertex) * vertexBuffer.size());

    // Free the device memory from the CPU
    vkUnmapMemory(vk::logialDevice, buff.mem);

    // Finally add this to the internal buffer register
    bufferStorageMap.push_back(buff);
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
