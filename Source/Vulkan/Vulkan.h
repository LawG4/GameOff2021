/*!********************************************************************************************************
\File          : Vulkan.h
\Copyright     : MIT License
\Brief         : Adds a header for using Vulkan
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once
#pragma warning(disable : 26812)  // Vulkan has an annoying prefer class enum over enum warning

// Use GLFW to include Vulkan headers
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Log.h"
#include "VulkanObjects.h"

/// <summary> A struct to hold all of the information required to start drawing a display list in Vulkan. ie
/// graphics pipelines </summary>
typedef struct VulkanDisplayList_t {
} VulkanDisplayList;

/// <summary> Starts Vulkan, including loading the function pointers </summary>
/// <returns>True if nothing went wrong</returns>
bool initVulkan();

/// <summary> Destorys all Vulkan objects </summary>
void cleanupVulkan();

namespace vk
{
/// <summary> The instance extensions that the user would like to enable and are supported</summary>
extern std::vector<const char*> validatedRequestedInstanceExtensions;

/// <summary> The instance extension properties supported by the vulkan instance </summary>
extern std::vector<VkExtensionProperties> instanceExtensionProperties;

/// <summary> The instance layers that have been validated as supported </summary>
extern std::vector<const char*> validatedRequestedInstanceLayers;

/// <summary> The layers supported by this instance </summary>
extern std::vector<VkLayerProperties> instanceLayerProperties;

/// <summary> Adds validation layers and extensions to the list of requested extensions and layers </summary>
void addValidationLayersAndExtensions(std::vector<const char*>& extensions, std::vector<const char*>& layers);

/// <summary> Have we decided to use validation layers? </summary>
extern bool validationLayersEnabled;

/// <summary> Tries to make a vulkan instance </summary>
/// <returns>True if successful</returns>
bool createInstance();

/// <summary> Attempts to create the vulkan debug callback messenger </summary>
void createDebugMessenger();

/// <summary> Deletes the debug messenger </summary>
void deleteDebugMessenger();

/// <summary> Selects the best Vulkan physical device </summary>
/// <returns>True if successful</returns>
bool selectBestPhysicalDevice();

/// <summary>The currently bound physical device's device extension properties </summary>
extern std::vector<VkExtensionProperties> deviceExtensionProperties;

/// <summary>A list of required device extensions, fail if a device doesn't have these </summary>
extern std::vector<const char*> requiredDeviceExtensions;

/// <summary>A list of device extensions it would be nice to have, use to rate best physical device </summary>
extern std::vector<const char*> requestedDeviceExtensions;

/// <summary> Creates a logical device from the selected physical device </summary>
/// <returns>True if successful</returns>
bool createLogicalDevice();

/// <summary> Creates the swapchain suitable for the window </summary>
/// <returns>returns if successful</returns>
bool createSwapchain();

/// <summary> Retrieves the onscreen renderpass </summary>
/// <returns>True if successful</returns>
bool createOnScreenRenderpass();

/// <summary> Creates the shader modules required for the basic pipeline </summary>
/// <returns>True if successful</returns>
bool createShaderModules();

/// <summary> Creates the graphics pipeline for this demo </summary>
/// <returns>True if successful</returns>
bool createGraphicsPipeline();

/// <summary>Creates a framebuffer from the swapchain image views </summary>
/// <returns>True if successful</returns>
bool createFramebuffer();
}  // namespace vk
