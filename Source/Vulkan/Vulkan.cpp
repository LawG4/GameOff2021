/*!********************************************************************************************************
\File          : Vulkan.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the function definition for initing Vulkan
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Vulkan.h"
#include "Memory.h"
#include "Pipelines.h"
#include "Window.h"
bool vulkanInitialised = false;

bool initVulkan()
{
    if (!vk::createInstance()) {
        Log.error("Could not create Vulkan instance");
        return false;
    } else {
        Log.info("Created Vulkan instance");
    }

    // Use glfw to create the surface for us
    if (glfwCreateWindowSurface(vk::instance, window, nullptr, &vk::surface) != VK_SUCCESS) {
        Log.error("Unable to create surface using glfw");
        return false;
    } else {
        Log.info("Created Vulkan surface");
    }

    if (!vk::selectBestPhysicalDevice()) {
        Log.error("Could not create a Vulkan physical device");
        return false;
    } else {
        Log.info("Created Vulkan Physical Device");
    }

    if (!vk::createLogicalDevice()) {
        Log.error("Could not create Vulkan logical device");
        return false;
    } else {
        Log.info("Created Vulkan Logical Device");
    }

    if (!vk::createSwapchain()) {
        Log.error("Could not create Vulkan Swapchain");
        return false;
    } else {
        Log.info("Created Vulkan swapchain");
    }

    if (!vk::createOnScreenRenderpass()) {
        Log.error("Could not create Vukan Renderpass");
        return false;
    } else {
        Log.info("Created Vulkan renderpass");
    }

    if (!vk::createFramebuffer()) {
        Log.error("Could not create framebuffers");
        return false;
    } else {
        Log.info("Created Vulkan framebuffers");
    }

    if (!vk::createCommandPools()) {
        Log.error("Could not create Vulkan command pools");
        return false;
    } else {
        Log.info("Created Vulkan command pool");
    }

    if (!vk::createDescriptorPoolAndSets()) {
        Log.error("Could not create descriptor pools");
        return false;
    } else {
        Log.info("Created Descriptor set and pool");
    }
    vk::createDescriptorSetLayouts();
    vk::createPipelines();

    if (!vk::allocateCommandBuffers()) {
        Log.error("Could not allocate Vulkan command buffers");
        return false;
    } else {
        Log.info("Allocated Vulkan command buffers");
    }

    if (!vk::createSyncObjects()) {
        Log.error("Could not create Vulkan semaphores");
        return false;
    } else {
        Log.info("Created Vulkan semaphores");
    }

    vulkanInitialised = true;
    return true;
}

void cleanupVulkan()
{
    if (!vulkanInitialised) return;

    // wait for the device to finish everything up
    vkDeviceWaitIdle(vk::logicalDevice);

    vk::destroyDescriptorResources();
    vk::destroyBuffers();

    for (uint32_t i = 0; i < vk::swapLength; i++) {
        vkDestroySemaphore(vk::logicalDevice, vk::readyForRendering[i], nullptr);
        vkDestroySemaphore(vk::logicalDevice, vk::finishedRendering[i], nullptr);
        vkDestroyFence(vk::logicalDevice, vk::inFlightCMDFence[i], nullptr);

        vkDestroyCommandPool(vk::logicalDevice, vk::graphicsPools.at(i), nullptr);
    }

    vkDestroyDescriptorPool(vk::logicalDevice, vk::descriptorPool, nullptr);

    vk::destroyPipelines(true);
    vk::destroyDescriptorSetLayouts();

    for (VkFramebuffer& fb : vk::swapchainFb) {
        vkDestroyFramebuffer(vk::logicalDevice, fb, nullptr);
    }

    vkDestroyRenderPass(vk::logicalDevice, vk::onscreenRenderPass, nullptr);

    for (VkImageView& view : vk::swapchainImageViews) {
        vkDestroyImageView(vk::logicalDevice, view, nullptr);
    }

    vkDestroySwapchainKHR(vk::logicalDevice, vk::swapchain, nullptr);

    vkDestroySurfaceKHR(vk::instance, vk::surface, nullptr);

    vkDestroyDevice(vk::logicalDevice, nullptr);

    if (vk::validationLayersEnabled) {
        vk::deleteDebugMessenger();
    }

    vkDestroyInstance(vk::instance, nullptr);

    Log.info("Vulkan Destoryed");
}
