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

    vulkanInitialised = true;
    return true;
}

void cleanupVulkan()
{
    if (!vulkanInitialised) return;

    vkDestroySurfaceKHR(vk::instance, vk::surface, nullptr);

    vkDestroyDevice(vk::logialDevice, nullptr);

    if (vk::validationLayersEnabled) {
        vk::deleteDebugMessenger();
    }

    vkDestroyInstance(vk::instance, nullptr);

    Log.info("Vulkan Destoryed");
}
