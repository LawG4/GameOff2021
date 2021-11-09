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

    return true;
}
