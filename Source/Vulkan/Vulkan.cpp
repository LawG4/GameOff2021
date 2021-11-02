/*!********************************************************************************************************
\File          : Vulkan.cpp
\Copyright     : MIT License
\Brief         : Provides the function definition for initing Vulkan
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Vulkan.h"

bool isVulkanSuitable() { return false; }

bool initVulkan()
{
    if (!glfwVulkanSupported()) {
        Log.error("GLFW could not find a Vulkan loader and Vulkan ICD");
        return false;
    } else {
        Log.info("A Vulkan loader and minimally capable ICD were found");
    }
}
