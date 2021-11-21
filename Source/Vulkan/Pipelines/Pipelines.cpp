/*!********************************************************************************************************
\File          : Pipelines.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides utility for recording all the different pipelines
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Pipelines.h"
#include "Sprites.h"

void vk::createPipelines()
{
    PipelineInternals::Templates::initialiseTemplates();
    PipelineInternals::create2DPipeline();
}

void vk::createDescriptorSetLayouts() { PipelineInternals::createDescriptorSetLayouts2D(); }

void vk::destroyDescriptorSetLayouts() { PipelineInternals::destroyDescriptorSetLayouts2D(); }

void vk::destroyPipelines() { PipelineInternals::destroy2DPipeline(); }

void vk::prepareCommandBuffer(uint32_t swapIndex)
{
    // Reset the graphics pool at the current swap index as it's cheap
    vkResetCommandPool(vk::logicalDevice, vk::graphicsPools.at(swapIndex),
                       VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);

    // Start recording the primary command buffer
    VkCommandBufferBeginInfo begin{};
    begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(vk::cmdBuffers.at(swapIndex), &begin);

    // Renderpasses must be started in the primary command buffer not secondary ones
    {
        VkRenderPassBeginInfo info{};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.framebuffer = vk::swapchainFb[swapIndex];
        info.renderPass = onscreenRenderPass;
        info.renderArea.extent = vk::swapchainExtent;
        info.renderArea.offset = {0, 0};

        VkClearValue clear = {0.0094117f, 0.00078431f, 0.00392156f, 1.0f};
        info.pClearValues = &clear;
        info.clearValueCount = 1;

        vkCmdBeginRenderPass(vk::cmdBuffers[swapIndex], &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record the sprite pipeline
    SpriteInternals::recordSpritePipeline(vk::cmdBuffers[swapIndex], swapIndex);

    // End the onscreen renderpass
    vkCmdEndRenderPass(vk::cmdBuffers[swapIndex]);

    // We're done recording so end the buffer
    vkEndCommandBuffer(vk::cmdBuffers.at(swapIndex));
}
