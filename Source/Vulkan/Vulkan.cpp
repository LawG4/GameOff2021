/*!********************************************************************************************************
\File          : Vulkan.cpp
\Copyright     : MIT License
\Brief         : Provides the function definition for initing Vulkan
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Vulkan.h"

bool vulkanInitialised = false;

bool initVulkan()
{
    if (!vk::createInstance()) {
        Log.error("Could not create Vulkan instance");
        return false;
    } else {
        Log.info("Created Vulkan instance");
    }

    if (!vk::selectBestPhysicalDevice()) {
        Log.error("Could not create a Vulkan physical device");
        return false;
    } else {
        Log.info("Created Vulkan Physical Device");
    }

    vulkanInitialised = true;
    return true;
}

void cleanupVulkan()
{
    if (!vulkanInitialised) return;

    if (vk::validationLayersEnabled) {
        vk::deleteDebugMessenger();
    }

    vkDestroyInstance(vk::instance, nullptr);

    Log.info("Vulkan Destoryed");
}
