/*!********************************************************************************************************
\File          : Vulkan.h
\Copyright     : MIT License
\Brief         : Header for declaring all the Vulkan objects
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once

#include "Vulkan.h"

namespace vk
{
extern VkInstance instance;
extern VkDebugUtilsMessengerEXT messenger;

extern VkPhysicalDevice physicalDevice;
}  // namespace vk
