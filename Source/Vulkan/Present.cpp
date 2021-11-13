/*!********************************************************************************************************
\File          : Present.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides presenting the final image to the swapchain
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Memory.h"
#include "Pipelines.h"
#include "Vulkan.h"

uint32_t currentFrame = 0;

// The semaphores for presenting
std::vector<VkSemaphore> vk::readyForRendering;
std::vector<VkSemaphore> vk::finishedRendering;

// Fences for syncing with the CPU
std::vector<VkFence> vk::inFlightCMDFence;
std::vector<VkFence> vk::inFlightImageFence;

// temp stuff to spin this triangle
#include <glm/gtc/matrix_transform.hpp>
glm::mat4 transform = glm::identity<glm::mat4>();

bool vk::createSyncObjects()
{
    vk::readyForRendering.resize(vk::swapLength);
    vk::finishedRendering.resize(vk::swapLength);
    vk::inFlightCMDFence.resize(vk::swapLength);
    vk::inFlightImageFence.resize(vk::swapLength, VK_NULL_HANDLE);

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

        if (vkCreateFence(vk::logialDevice, &fence, nullptr, &vk::inFlightCMDFence[i]) != VK_SUCCESS) {
            Log.error("Couldn't create a fence");
            return false;
        }
    }

    return true;
}

bool vk::drawFrame()
{
    // Wait on the CPU side until the GPU work for the current frame has finished
    vkWaitForFences(vk::logialDevice, 1, &vk::inFlightCMDFence[currentFrame], VK_TRUE,
                    static_cast<uint64_t>(-1));

    uint32_t imageIndex = 0;
    VkResult result;

    // Request the next image in the swapchain
    result = vkAcquireNextImageKHR(vk::logialDevice, vk::swapchain, static_cast<uint64_t>(-1),
                                   vk::readyForRendering[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        Log.info("Attempt to recreate the swapchain");
        recreateSwapchain();
        return true;

    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        Log.error("Failed to retrieve an image from the swapchain");
        return false;
    }

    // The image we retrieved from the swapchain might be out of order, so we need to wait on the image to be
    // ready, so if we have a fence set then wait on it
    if (vk::inFlightImageFence[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(vk::logialDevice, 1, &vk::inFlightImageFence[imageIndex], VK_TRUE,
                        static_cast<uint64_t>(-1));
    }

    // Assign the current swapchain image as being used by this frame, so that we can tag the correct fence
    vk::inFlightImageFence[imageIndex] = inFlightCMDFence[currentFrame];

    // Now it is most likely that we have to record the command buffer every single frame
    prepareCommandBuffer(imageIndex);

    // Now that we have a frame ready, draw to it by submitting the command buffer
    VkSubmitInfo submit;
    memset(&submit, 0, sizeof(VkSubmitInfo));
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    // Tell Vulkan which semaphore to wait to be signaled
    // In this case we're telling Vulkan to wait until the ready for rendering semaphore has been signalled
    // before running the command buffer
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &vk::readyForRendering[currentFrame];
    submit.pWaitDstStageMask = &waitStage;

    // Tell vulkan which semaphore to signal once the command buffer has finished rendering
    // in this case we're telling Vulkan to signal finishedRendering once the command buffer is done
    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &vk::finishedRendering[currentFrame];

    // The command buffer we're submitting
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &vk::cmdBuffers.at(imageIndex);

    // Reset the fence just before submitting the command buffer
    vkResetFences(vk::logialDevice, 1, &vk::inFlightCMDFence[currentFrame]);

    // SubGPL-3.0 Licensethe command queue
    if (vkQueueSubmit(vk::graphicsQueue, 1, &submit, vk::inFlightCMDFence[currentFrame]) != VK_SUCCESS) {
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
    present.pWaitSemaphores = &vk::finishedRendering[currentFrame];

    // Advance Frame index
    currentFrame = (currentFrame + 1) % vk::swapLength;

    vkQueuePresentKHR(vk::presentationQueue, &present);

    // now rotate the temp matrix a little bit
    transform = glm::rotate(transform, 0.2f, glm::vec3(0.0, 0.0, 1.0));

    return true;
}
