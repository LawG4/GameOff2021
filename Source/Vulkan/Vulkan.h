/*!********************************************************************************************************
\File          : Vulkan.h
\Copyright     : MIT License
\Brief         : Adds a header for using Vulkan
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once
#include "Log.h"
#include "vulkan/vulkan.h"

#include "Window.h"

/// <summary> A struct to hold all of the information required to start drawing a display list in Vulkan. ie
/// graphics pipelines </summary>
typedef struct VulkanDisplayList_t {
} VulkanDisplayList;

/// <summary> Tests to see if this computer should use Vulkan or not </summary>
/// <returns>True if we should use Vulkan</returns>
bool isVulkanSuitable();

/// <summary> Starts Vulkan, including loading the function pointers </summary>
/// <returns>True if nothing went wrong</returns>
bool initVulkan();
