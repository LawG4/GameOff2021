/*!********************************************************************************************************
\File          : Surface
\Copyright     : MIT License
\Brief         : Handels surface and swapchain creation and recreation
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Vulkan.h"
#include "Window.h"

#include <algorithm>

VkSurfaceKHR vk::surface;
VkSwapchainKHR vk::swapchain;
uint32_t vk::swapLength;
SwapchainProperties vk::selectedSwapchainProperties;
VkFormat vk::swapchainFormat;
VkExtent2D vk::swapchainExtent;
std::vector<VkImage> vk::swapchainImages;

VkPresentModeKHR chooseSwapchainPresentMode();
VkSurfaceFormatKHR chooseSwapchainFormat(std::vector<VkSurfaceFormatKHR>& formats);
VkExtent2D chooseSwapchainExtent(VkSurfaceCapabilitiesKHR& capabilities);

bool vk::createSwapchain()
{
    VkSwapchainCreateInfoKHR createInfo;
    memset(&createInfo, 0, sizeof(VkSwapchainCreateInfoKHR));

    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vk::surface;

    // Set the swapchain image format
    VkSurfaceFormatKHR format = chooseSwapchainFormat(vk::selectedSwapchainProperties.formats);
    createInfo.imageFormat = format.format;
    createInfo.imageColorSpace = format.colorSpace;
    vk::swapchainFormat = format.format;

    // Set the swapchain present mode
    createInfo.presentMode = chooseSwapchainPresentMode();

    // Set the extent of the swapchain
    createInfo.imageExtent = chooseSwapchainExtent(vk::selectedSwapchainProperties.capabilities);
    vk::swapchainExtent = createInfo.imageExtent;

    // Choose how many images we'll have in the swapchain
    vk::swapLength = vk::selectedSwapchainProperties.capabilities.minImageCount;
    swapLength = std::clamp(vk::swapLength + 1, vk::swapLength,
                            vk::selectedSwapchainProperties.capabilities.maxImageCount);
    createInfo.minImageCount = vk::swapLength;

    // Get the swqpchain pretransform from the surface capabilities
    createInfo.preTransform = vk::selectedSwapchainProperties.capabilities.currentTransform;

    // Set default information about a 2d swapchain
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.clipped = VK_TRUE;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    // We're not recreating a swapchain thankfully
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(vk::logialDevice, &createInfo, nullptr, &vk::swapchain) != VK_SUCCESS) {
        Log.error("Could not create a swapchain");
        return false;
    }

    // Retrieve the Vulkan swapchain image
    vkGetSwapchainImagesKHR(vk::logialDevice, vk::swapchain, &vk::swapLength, nullptr);
    vk::swapchainImages.resize(vk::swapLength);
    vkGetSwapchainImagesKHR(vk::logialDevice, vk::swapchain, &vk::swapLength, vk::swapchainImages.data());

    return true;
}

VkPresentModeKHR chooseSwapchainPresentMode()
{
    // Fifo mode is guarenteed
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR chooseSwapchainFormat(std::vector<VkSurfaceFormatKHR>& formats)
{
    // Look for SRGB if not just return the first one in the list
    for (auto& format : formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    Log.warn("Couldn't find sRGB format for the swapchain");
    return formats.at(0);
}

VkExtent2D chooseSwapchainExtent(VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != static_cast<uint32_t>(-1)) {
        Log.info("Already have a viable extent");
        return capabilities.currentExtent;
    }

    // Get the framebuffer size from GLFW
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D extent;
    memset(&extent, 0, sizeof(VkExtent2D));

    // Clamp the extent to ensure it's not out of the supported bounds
    extent.width = std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width,
                              capabilities.minImageExtent.width);
    extent.height = std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height,
                               capabilities.maxImageExtent.height);
    Log.info("We're using an extent of size (x = {}, y = {})", extent.width, extent.height);
    return extent;
}

void SwapchainProperties::populate(VkPhysicalDevice device)
{
    // Clear everything
    memset(&this->capabilities, 0, sizeof(VkSurfaceCapabilitiesKHR));
    this->formats.clear();
    this->presentModes.clear();

    // Get the supported on screen formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk::surface, &formatCount, nullptr);
    formats.resize(formatCount);
    if (formatCount != 0) {
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk::surface, &formatCount, formats.data());
    }

    // Get the present modes
    uint32_t presentCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk::surface, &presentCount, nullptr);
    presentModes.resize(presentCount);
    if (presentCount != 0) {
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk::surface, &presentCount, presentModes.data());
    }

    // Now get the capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vk::surface, &capabilities);
}

bool SwapchainProperties::swapchainSuitable()
{
    // Check that present modes and formats are non empty
    if (presentModes.size() == 0 || formats.size() == 0) {
        Log.warn("Incompatable swapchain capabilities");
        return false;
    }

    return true;
}