/*!********************************************************************************************************
\File          : Decriptors.cpp
\Copyright     : GPL-3.0 License
\Brief         : Writes descriptor sets for pipeline buffer access
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Memory.h"
#include "Vulkan.h"

VkDescriptorPool vk::descriptorPool;

struct UniformBufferObject {
    glm::mat4 modelMatrix;
};

void vk::addUniformBuffer()
{
    // We should ass one uniform buffer for image in the swapchain so that buffers in flight don't get
    // overwritten
    vk::DescriptorGroup desc;
    desc.buffers.resize(vk::swapLength);
    desc.descSets.resize(vk::swapLength);

    // Create one buffer per swapchain image
    VkDeviceSize size = sizeof(UniformBufferObject);
    for (uint32_t i = 0; i < vk::swapLength; i++) {
        desc.buffers.at(i) =
          vk::createBufferGroup(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    // Create a descriptor set layout
    VkDescriptorSetLayoutCreateInfo layout;
    memset(&layout, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
    layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

    // We have one ubo in binding 0
    VkDescriptorSetLayoutBinding binding;
    memset(&binding, 0, sizeof(VkDescriptorSetLayoutBinding));
    binding.binding = 0;
    binding.descriptorCount = 1;
    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    binding.stageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;

    layout.bindingCount = 1;
    layout.pBindings = &binding;

    if (vkCreateDescriptorSetLayout(vk::logialDevice, &layout, nullptr, &desc.layout) != VK_SUCCESS) {
        Log.error("Could not make descriptor set layout");
    }

    // Allocate one descriptor set per image in the swapchain
    VkDescriptorSetAllocateInfo alloc;
    memset(&alloc, 0, sizeof(VkDescriptorSetAllocateInfo));
    alloc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc.descriptorPool = vk::descriptorPool;

    // Use a trick of using an array of the same layouts to allocate multiple sets in one go
    std::vector<VkDescriptorSetLayout> layouts(vk::swapLength, desc.layout);
    alloc.descriptorSetCount = vk::swapLength;
    alloc.pSetLayouts = layouts.data();

    if (vkAllocateDescriptorSets(vk::logialDevice, &alloc, desc.descSets.data()) != VK_SUCCESS) {
        Log.error("Couldn't allocate descriptor sets");
        return;
    }

    VkWriteDescriptorSet write;
    memset(&write, 0, sizeof(VkWriteDescriptorSet));
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    std::vector<VkWriteDescriptorSet> descriptorWriter;
    for (uint32_t i = 0; i < vk::swapLength; i++) {
        VkDescriptorBufferInfo info;
        info.buffer = desc.buffers.at(i).buffer;
        info.offset = 0;
        info.range = VK_WHOLE_SIZE;

        write.pBufferInfo = &info;
        write.dstSet = desc.descSets.at(i);
        descriptorWriter.push_back(write);
    }

    vkUpdateDescriptorSets(vk::logialDevice, descriptorWriter.size(), descriptorWriter.data(), 0, nullptr);
}

bool vk::createDescriptorPoolAndSets()
{
    VkDescriptorPoolCreateInfo pool;
    memset(&pool, 0, sizeof(VkDescriptorPoolCreateInfo));
    pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

    // Tell the descriptor pool about the number of sets we expect to have
    VkDescriptorPoolSize poolSize;
    poolSize.descriptorCount = vk::swapLength;
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool.poolSizeCount = 1;
    pool.pPoolSizes = &poolSize;

    // Set the maximum number of descriptor sets in this pool
    pool.maxSets = vk::swapLength;

    if (vkCreateDescriptorPool(vk::logialDevice, &pool, nullptr, &vk::descriptorPool) != VK_SUCCESS) {
        Log.error("Could not create the descriptor pool");
        return false;
    }

    // create our temp uniform buffer
    vk::addUniformBuffer();

    return true;
}
