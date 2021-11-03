/*!********************************************************************************************************
\File          : Vulkan.h
\Copyright     : MIT License
\Brief         : Adds a header for using Vulkan
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once

// Use GLFW to include Vulkan headers
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Log.h"
#include "VulkanObjects.h"

/// <summary> Dynamically loads Vulkan library </summary>
/// <returns> True if vulkan loaded succesfully</returns>
bool osDynamicLoadVulkan();

/// <summary> A struct to hold all of the information required to start drawing a display list in Vulkan. ie
/// graphics pipelines </summary>
typedef struct VulkanDisplayList_t {
} VulkanDisplayList;

/// <summary> Starts Vulkan, including loading the function pointers </summary>
/// <returns>True if nothing went wrong</returns>
bool initVulkan();

namespace vk
{

bool createInstance();
}  // namespace vk
