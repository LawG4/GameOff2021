/*!********************************************************************************************************
\File          : Memory.h
\Copyright     : GPL-3.0 License
\Brief         : Provides utility for allocating device memory
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once

#include "Vertex.h"
#include "Vulkan.h"

#include <unordered_map>

namespace vk
{

struct BufferGroup {
    VkBuffer buffer;
    VkBufferView view;
    VkDeviceMemory mem;
};

extern std::vector<vk::BufferGroup> bufferStorageMap;

/// <summary> Creates a vertex buffer with the initial contents </summary>
/// <param name="bufferName"> The name for the buffer in the internal array</param>
/// <param name="vertexBuffer"> An array of vertices</param>
void addVertexBuffer(const char *bufferName, const std::vector<Vertex> &vertexBuffer);

/// <summary> Destorys all of the buffers in the hash table </summary>
void destroyBuffers();

namespace memory
{
extern VkPhysicalDeviceMemoryProperties memProperties;
}
}  // namespace vk
