/*!********************************************************************************************************
\File          : CommandBuffer.cpp
\Copyright     : MIT License
\Brief         : Created command buffers and pools
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"

VkCommandPool vk::graphicsPool;
std::vector<VkCommandBuffer> vk::cmdBuffers;

bool recordCommandBuffers()
{
    // Set the clear colour for our onscreen attachment
    VkClearValue clear = {0.0f, 0.0f, 0.0f, 1.0f};

    VkCommandBufferBeginInfo begin;
    memset(&begin, 0, sizeof(VkCommandBufferBeginInfo));
    begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    // Start by begining the render pass
    VkRenderPassBeginInfo renderpass;
    memset(&renderpass, 0, sizeof(VkRenderPassBeginInfo));
    renderpass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderpass.clearValueCount = 1;
    renderpass.pClearValues = &clear;

    // Attach to onscreen renderpass
    renderpass.renderPass = vk::onscreenRenderPass;
    renderpass.renderArea.extent = vk::swapchainExtent;
    renderpass.renderArea.offset = {0, 0};

    // The things that change per frame in swapchain
    for (uint32_t i = 0; i < vk::swapLength; i++) {
        VkCommandBuffer &cmd = vk::cmdBuffers.at(i);

        // begin the command buffer recording
        if (vkBeginCommandBuffer(cmd, &begin) != VK_SUCCESS) {
            Log.error("Could not start recording the command buffer");
            return false;
        }

        // begin the renderpass
        renderpass.framebuffer = vk::swapchainFb.at(i);
        vkCmdBeginRenderPass(cmd, &renderpass, VK_SUBPASS_CONTENTS_INLINE);

        // Bind the graphics pipeline
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vk::graphicsPipeline);

        vkCmdDraw(cmd, 3, 1, 0, 0);

        vkCmdEndRenderPass(cmd);

        if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
            Log.error("Failed to finish recording the command buffer");
            return false;
        }
    }

    return true;
}

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

    if (!recordCommandBuffers()) {
        Log.error("Failed to record command buffer");
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
