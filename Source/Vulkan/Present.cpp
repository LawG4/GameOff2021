/*!********************************************************************************************************
\File          : Present.cpp
\Copyright     : MIT License
\Brief         : Provides presenting the final image to the swapchain
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"

// The semaphores for presenting don't need to be availble to other vulkan objects (right now)
VkSemaphore vk::readyForRendering;
VkSemaphore vk::finishedRendering;

bool vk::createSemaphores()
{
    VkSemaphoreCreateInfo semaphore;
    memset(&semaphore, 0, sizeof(VkSemaphoreCreateInfo));
    semaphore.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(vk::logialDevice, &semaphore, nullptr, &vk::readyForRendering) != VK_SUCCESS ||
        vkCreateSemaphore(vk::logialDevice, &semaphore, nullptr, &vk::finishedRendering) != VK_SUCCESS) {
        Log.error("Could not create semaphores");
        return false;
    }

    return true;
}

bool vk::drawFrame()
{
    // Wait for the next frame to be availbe for rendering
    uint32_t imageIndex;
    vkAcquireNextImageKHR(vk::logialDevice, vk::swapchain, static_cast<uint64_t>(-1), vk::readyForRendering,
                          VK_NULL_HANDLE, &imageIndex);
    Log.info("Image index is : {}", imageIndex);

    // Now that we have a frame ready, draw to it by submitting the command buffer
    VkSubmitInfo submit;
    memset(&submit, 0, sizeof(VkSubmitInfo));
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // Tell Vulkan which semaphore to wait to be signaled
    // In this case we're telling Vulkan to wait until the ready for rendering semaphore has been signalled
    // before running the command buffer
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &vk::readyForRendering;
    submit.pWaitDstStageMask = &waitStage;

    // Tell vulkan which semaphore to signal once the command buffer has finished rendering
    // in this case we're telling Vulkan to signal finishedRendering once the command buffer is done
    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &vk::finishedRendering;

    // The command buffer we're submitting
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &vk::cmdBuffers.at(imageIndex);

    // Submit the command buffer
    if (vkQueueSubmit(vk::graphicsQueue, 1, &submit, VK_NULL_HANDLE) != VK_SUCCESS) {
        Log.error("Failed to submit graphics queue");
        return false;
    }

    // Once the command buffer has been submitted, we wait for the finishedRendering semaphore to be signalled
    // by the ending command buffer, and then we start presenting the finished image
    VkPresentInfoKHR present;
    memset(&present, 0, sizeof(VkPresentInfoKHR));
    present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present.pSwapchains = &vk::swapchain;
    present.swapchainCount = 1;
    present.pImageIndices = &imageIndex;
    present.waitSemaphoreCount = 1;
    present.pWaitSemaphores = &vk::finishedRendering;

    vkQueuePresentKHR(vk::presentationQueue, &present);

    return true;
}
