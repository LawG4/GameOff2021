/*!********************************************************************************************************
\File          : Vulkan.cpp
\Copyright     : MIT License
\Brief         : Provides the function definition for initing Vulkan
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Vulkan.h"
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

    if (!vk::createShaderModules()) {
        Log.error("Could not create shader modules");
        return false;
    }

    if (!vk::createGraphicsPipeline()) {
        Log.error("Could not create the graphics pipeline");
        return false;
    } else {
        Log.info("Created Vulkan graphics pipeline");
    }

    vulkanInitialised = true;
    return true;
}

void cleanupVulkan()
{
    if (!vulkanInitialised) return;

    vkDestroyPipeline(vk::logialDevice, vk::graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(vk::logialDevice, vk::graphicsLayout, nullptr);

    vkDestroyShaderModule(vk::logialDevice, vk::vertModule, nullptr);
    vkDestroyShaderModule(vk::logialDevice, vk::fragModule, nullptr);

    for (VkFramebuffer& fb : vk::swapchainFb) {
        vkDestroyFramebuffer(vk::logialDevice, fb, nullptr);
    }

    vkDestroyRenderPass(vk::logialDevice, vk::onscreenRenderPass, nullptr);

    for (VkImageView& view : vk::swapchainImageViews) {
        vkDestroyImageView(vk::logialDevice, view, nullptr);
    }

    vkDestroySwapchainKHR(vk::logialDevice, vk::swapchain, nullptr);

    vkDestroySurfaceKHR(vk::instance, vk::surface, nullptr);

    vkDestroyDevice(vk::logialDevice, nullptr);

    if (vk::validationLayersEnabled) {
        vk::deleteDebugMessenger();
    }

    vkDestroyInstance(vk::instance, nullptr);

    Log.info("Vulkan Destoryed");
}
