/*!********************************************************************************************************
\File          : Vulkan.cpp
\Copyright     : MIT License
\Brief         : Provides the function definition for initing Vulkan
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Vulkan.h"

bool initVulkan()
{
    if (!vk::createInstance()) {
        Log.error("Could not create Vulkan instance");
        return false;
    } else {
        Log.info("Created Vulkan instance");
    }

    return true;
}
