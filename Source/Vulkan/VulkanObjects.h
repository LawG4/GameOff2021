/*!********************************************************************************************************
\File          : Vulkan.h
\Copyright     : GPL-3.0 License
\Brief         : Header for declaring all the Vulkan objects
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once

#include <optional>
#include "Vulkan.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool allQueuesPresent();
};

struct SwapchainProperties {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    /// <summary> Takes a pysical device and fills this swapchain propeties as a result </summary>
    /// <param name="device">The device we're populating</param>
    void populate(VkPhysicalDevice device);

    /// <summary> Does the selected device have enough swapchain capabilities? </summary>
    /// <returns>True if swapchain support is good enough</returns>
    bool swapchainSuitable();
};

namespace vk
{
extern VkInstance instance;
extern VkDebugUtilsMessengerEXT messenger;

extern VkPhysicalDevice physicalDevice;
extern QueueFamilyIndices selectedQueueFamilies;
extern VkDevice logialDevice;

extern VkQueue graphicsQueue;
extern VkQueue presentationQueue;

extern VkSurfaceKHR surface;
extern VkSwapchainKHR swapchain;
extern uint32_t swapLength;
extern SwapchainProperties selectedSwapchainProperties;
extern VkFormat swapchainFormat;
extern VkExtent2D swapchainExtent;
extern std::vector<VkImage> swapchainImages;
extern std::vector<VkImageView> swapchainImageViews;
extern std::vector<VkFramebuffer> swapchainFb;

extern VkRenderPass onscreenRenderPass;

extern VkShaderModule vertModule;
extern VkShaderModule fragModule;
extern VkPipeline graphicsPipeline;
extern VkPipelineLayout graphicsLayout;

extern VkCommandPool graphicsPool;
extern std::vector<VkCommandBuffer> cmdBuffers;

extern std::vector<VkSemaphore> readyForRendering;
extern std::vector<VkSemaphore> finishedRendering;
extern std::vector<VkFence> inFlightCMDFence;
extern std::vector<VkFence> inFlightImageFence;
}  // namespace vk
