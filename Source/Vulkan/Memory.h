/*!********************************************************************************************************
\File          : Memory.h
\Copyright     : GPL-3.0 License
\Brief         : Provides utility for allocating device memory
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once

#include "Vertex.h"
#include "Vulkan.h"
#include "VulkanObjects.h"

#include <unordered_map>

struct UniformBufferObject {
    glm::mat4 modelMatrix;
};

namespace vk
{

extern std::vector<vk::BufferGroup> bufferStorageMap;

extern std::vector<vk::DescriptorGroup> descriptorStorageMap;

/// <summary>Creates a Vulkan buffer and allocates device memory</summary>
/// <param name="size">The size of the memory to allocate</param>
/// <param name="usage">The VkBuffer usage flags</param>
/// <param name="properties">The memory propertys required</param>
/// <returns>A buffer group which has a buffer and device memory</returns>
vk::BufferGroup createBufferGroup(VkDeviceSize size, VkBufferUsageFlags usage,
                                  VkMemoryPropertyFlags properties);

vk::BufferGroup createVertexBufferGroup(VkDeviceSize size, void *data);

/// <summary> Creates a vertex buffer with the initial contents </summary>
/// <param name="bufferName"> The name for the buffer in the internal array</param>
/// <param name="vertexBuffer"> An array of vertices</param>
void addVertexBuffer(const char *bufferName, const std::vector<Vertex> &vertexBuffer);

/// <summary>Finds the index of the GPU heap that has all the requirements to allocate some memory</summary>
/// <param name="typeFilter">I actually have no idea</param>
/// <param name="properties">Memory properties determined by vkGetMemoryRequirements</param>
/// <returns>Appropriate heap index</returns>
uint32_t findMemoryIndex(uint32_t typeFilter, VkMemoryPropertyFlags properties);

/// <summary> Destorys all of the buffers in the hash table </summary>
void destroyBuffers();

/// <summary> Writes the temporary uniform buffer </summary>
void addUniformBuffer();

/// <summary>Updates a vertex buffer for the current swap index </summary>
void updateUniformAtSwapIndex(vk::DescriptorGroup desc, uint32_t swapIndex, UniformBufferObject obj);

/// <summary> Destroys buffers with a descriptor set </summary>
void destroyDescriptorResources();

namespace memory
{
extern VkPhysicalDeviceMemoryProperties memProperties;
}
}  // namespace vk
