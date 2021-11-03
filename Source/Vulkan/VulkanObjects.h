/*!********************************************************************************************************
\File          : Vulkan.h
\Copyright     : MIT License
\Brief         : Header for declaring all the Vulkan objects
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once

#include <optional>
#include "Vulkan.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
};

namespace vk
{
extern VkInstance instance;
extern VkDebugUtilsMessengerEXT messenger;

extern VkPhysicalDevice physicalDevice;
extern QueueFamilyIndices selectedQueueFamilies;
extern VkDevice logialDevice;

extern VkQueue graphicsQueue;

extern VkSurfaceKHR surface;
}  // namespace vk
