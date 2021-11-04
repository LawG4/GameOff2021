/*!********************************************************************************************************
\File          : Present.cpp
\Copyright     : MIT License
\Brief         : Provides presenting the final image to the swapchain
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"

uint32_t currentFrameIndex = 0;

// The semaphores for presenting
std::vector<VkSemaphore> vk::readyForRendering;
std::vector<VkSemaphore> vk::finishedRendering;

// Fences for syncing with the CPU
std::vector<VkFence> vk::inFlightFence;

bool vk::createSyncObjects()
{
    vk::readyForRendering.resize(vk::swapLength);
    vk::finishedRendering.resize(vk::swapLength);
    vk::inFlightFence.resize(vk::swapLength);

    VkSemaphoreCreateInfo semaphore;
    memset(&semaphore, 0, sizeof(VkSemaphoreCreateInfo));
    semaphore.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence;
    memset(&fence, 0, sizeof(VkFenceCreateInfo));
    fence.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (uint32_t i = 0; i < vk::swapLength; i++) {
        if (vkCreateSemaphore(vk::logialDevice, &semaphore, nullptr, &vk::readyForRendering[i]) !=
              VK_SUCCESS ||
            vkCreateSemaphore(vk::logialDevice, &semaphore, nullptr, &vk::finishedRendering[i]) !=
              VK_SUCCESS) {
            Log.error("Could not create semaphores");
            return false;
        }

        if (vkCreateFence(vk::logialDevice, &fence, nullptr, &vk::inFlightFence[i]) != VK_SUCCESS) {
            Log.error("Couldn't create a fence");
            return false;
        }
    }

    return true;
}

bool vk::drawFrame()
{
    // Wait on the CPU side for the fence to be ready
    vkWaitForFences(vk::logialDevice, 1, &vk::inFlightFence[currentFrameIndex], VK_TRUE,
                    static_cast<uint64_t>(-1));
    vkResetFences(vk::logialDevice, 1, &vk::inFlightFence[currentFrameIndex]);

    // Wait for the next frame to be availbe for rendering
    uint32_t imageIndex;
    vkAcquireNextImageKHR(vk::logialDevice, vk::swapchain, static_cast<uint64_t>(-1),
                          vk::readyForRendering[currentFrameIndex], VK_NULL_HANDLE, &imageIndex);

    // Now that we have a frame ready, draw to it by submitting the command buffer
    VkSubmitInfo submit;
    memset(&submit, 0, sizeof(VkSubmitInfo));
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // Tell Vulkan which semaphore to wait to be signaled
    // In this case we're telling Vulkan to wait until the ready for rendering semaphore has been signalled
    // before running the command buffer
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &vk::readyForRendering[currentFrameIndex];
    submit.pWaitDstStageMask = &waitStage;

    // Tell vulkan which semaphore to signal once the command buffer has finished rendering
    // in this case we're telling Vulkan to signal finishedRendering once the command buffer is done
    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &vk::finishedRendering[currentFrameIndex];

    // The command buffer we're submitting
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &vk::cmdBuffers.at(imageIndex);

    // Submit the command buffer
    if (vkQueueSubmit(vk::graphicsQueue, 1, &submit, vk::inFlightFence[currentFrameIndex]) != VK_SUCCESS) {
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
    present.pWaitSemaphores = &vk::finishedRendering[currentFrameIndex];

    // Advance Frame index
    currentFrameIndex = (currentFrameIndex + 1) % vk::swapLength;

    vkQueuePresentKHR(vk::presentationQueue, &present);

    return true;
}
