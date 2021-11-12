
#include "Pipelines.h"
void prepareCommandBuffer(uint32_t swapIndex)
{
    // Reset the graphics pool at the current swap index as it's cheap
    vkResetCommandPool(vk::logialDevice, vk::graphicsPools.at(swapIndex),
                       VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);

    // Start recording the primary command buffer
    VkCommandBufferBeginInfo begin{};
    begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(vk::cmdBuffers.at(swapIndex), &begin);

    // Record the 2D command buffer
    {
        VkClearValue clear = {0.0094117f, 0.00078431f, 0.00392156f, 1.0f};
        VkRenderPassBeginInfo renderpass;
        memset(&renderpass, 0, sizeof(VkRenderPassBeginInfo));
        renderpass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpass.clearValueCount = 1;
        renderpass.pClearValues = &clear;

        // Attach to onscreen renderpass
        renderpass.renderPass = vk::onscreenRenderPass;
        renderpass.renderArea.extent = vk::swapchainExtent;
        renderpass.renderArea.offset = {0, 0};

        // Begin the renderpass
        renderpass.framebuffer = vk::swapchainFb.at(swapIndex);
        vkCmdBeginRenderPass(vk::cmdBuffers.at(swapIndex), &renderpass, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdEndRenderPass(vk::cmdBuffers.at(swapIndex));
    }

    // We're done recording so reset the buffer
    vkEndCommandBuffer(vk::cmdBuffers.at(swapIndex));
}
