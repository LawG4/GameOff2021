/*!********************************************************************************************************
\File          : Surface
\Copyright     : GPL-3.0 License
\Brief         : Handels surface and swapchain creation and recreation
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Camera.h"
#include "Memory.h"
#include "Pipelines.h"
#include "Vulkan.h"
#include "Window.h"

#include <algorithm>
#include "glm/gtc/matrix_transform.hpp"

VkSurfaceKHR vk::surface;
VkSwapchainKHR vk::swapchain;
uint32_t vk::swapLength;
SwapchainProperties vk::selectedSwapchainProperties;
VkFormat vk::swapchainFormat;
VkExtent2D vk::swapchainExtent;
std::vector<VkImage> vk::swapchainImages;
std::vector<VkImageView> vk::swapchainImageViews;

std::vector<VkFramebuffer> vk::swapchainFb;

VkPresentModeKHR chooseSwapchainPresentMode();
VkSurfaceFormatKHR chooseSwapchainFormat(std::vector<VkSurfaceFormatKHR>& formats);
VkExtent2D chooseSwapchainExtent(VkSurfaceCapabilitiesKHR& capabilities);

std::vector<VkImage> depthImages;
std::vector<VkDeviceMemory> depthMemorys;
std::vector<VkImageView> depthViews;

bool vk::recreateSwapchain()
{
    // First check if we're minimised
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    // keep waiting until both are non zero
    while (!(width & height)) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    Log.info("Recreating Vulkan Swapchain");

    // Wait for device to finish any commands
    vkDeviceWaitIdle(vk::logicalDevice);

    // Destroy the resources
    for (uint32_t i = 0; i < vk::swapLength; i++) {
        // Per frame resources
        vkDestroyFramebuffer(vk::logicalDevice, vk::swapchainFb[i], nullptr);
        vkDestroyImageView(vk::logicalDevice, vk::swapchainImageViews[i], nullptr);
    }
    vk::destroyDepthAttachments();

    vk::destroyPipelines(false);
    vkDestroyRenderPass(vk::logicalDevice, vk::onscreenRenderPass, nullptr);
    vkDestroySwapchainKHR(vk::logicalDevice, vk::swapchain, nullptr);

    // recreate the lost resources
    vk::selectedSwapchainProperties.populate(vk::physicalDevice);
    vk::createSwapchain();
    vk::createOnScreenRenderpass();
    vk::createFramebuffer();
    vk::createPipelines();

    return true;
}

void createDepthAttachments()
{
    depthImages.resize(vk::swapLength);
    depthViews.resize(vk::swapLength);
    depthMemorys.resize(vk::swapLength);

    // Just assume that depth 32 is always supported
    VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;

    VkFormatProperties properties{};
    vkGetPhysicalDeviceFormatProperties(vk::physicalDevice, depthFormat, &properties);

    // Create the underlying depth images
    {
        VkImageCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        info.format = depthFormat;
        info.samples = VK_SAMPLE_COUNT_1_BIT;

        info.imageType = VK_IMAGE_TYPE_2D;
        info.extent.width = vk::swapchainExtent.width;
        info.extent.height = vk::swapchainExtent.height;
        info.extent.depth = 1;

        info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        info.mipLevels = 1;
        info.arrayLayers = 1;

        for (uint32_t i = 0; i < vk::swapLength; i++) {
            vkCreateImage(vk::logicalDevice, &info, nullptr, &depthImages[i]);

            // attach some memory to this image
            VkMemoryAllocateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

            VkMemoryRequirements req;
            vkGetImageMemoryRequirements(vk::logicalDevice, depthImages[i], &req);
            info.allocationSize = req.size;
            info.memoryTypeIndex =
              vk::findMemoryIndex(req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            vkAllocateMemory(vk::logicalDevice, &info, nullptr, &depthMemorys[i]);
            vkBindImageMemory(vk::logicalDevice, depthImages[i], depthMemorys[i], 0);
        }
    }

    // Create the image view
    {
        VkImageViewCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.format = depthFormat;
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;

        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        info.subresourceRange.layerCount = 1;
        info.subresourceRange.levelCount = 1;

        for (uint32_t i = 0; i < vk::swapLength; i++) {
            info.image = depthImages[i];

            vkCreateImageView(vk::logicalDevice, &info, nullptr, &depthViews[i]);
        }
    }
}

void vk::destroyDepthAttachments()
{
    for (uint32_t i = 0; i < vk::swapLength; i++) {
        vkDestroyImageView(vk::logicalDevice, depthViews[i], nullptr);
        vkFreeMemory(vk::logicalDevice, depthMemorys[i], nullptr);
        vkDestroyImage(vk::logicalDevice, depthImages[i], nullptr);
    }
}

bool vk::createFramebuffer()
{
    vk::swapchainFb.resize(vk::swapLength);

    createDepthAttachments();

    VkFramebufferCreateInfo framebuffer{};
    framebuffer.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer.attachmentCount = 2;
    framebuffer.renderPass = vk::onscreenRenderPass;
    framebuffer.width = vk::swapchainExtent.width;
    framebuffer.height = vk::swapchainExtent.height;
    framebuffer.layers = 1;

    for (uint32_t i = 0; i < vk::swapLength; i++) {
        // add all the attachments for the framebuffer in the correct order
        VkImageView attachments[2] = {vk::swapchainImageViews[i], depthViews[i]};
        framebuffer.pAttachments = attachments;

        if (vkCreateFramebuffer(vk::logicalDevice, &framebuffer, nullptr, &vk::swapchainFb.at(i)) !=
            VK_SUCCESS) {
            Log.error("Couldn't create framebuffer index {}", i);
            return true;
        }
    }

    return true;
}

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

    if (vkCreateSwapchainKHR(vk::logicalDevice, &createInfo, nullptr, &vk::swapchain) != VK_SUCCESS) {
        Log.error("Could not create a swapchain");
        return false;
    }

    // Retrieve the Vulkan swapchain image
    vkGetSwapchainImagesKHR(vk::logicalDevice, vk::swapchain, &vk::swapLength, nullptr);
    vk::swapchainImages.resize(vk::swapLength);
    vkGetSwapchainImagesKHR(vk::logicalDevice, vk::swapchain, &vk::swapLength, vk::swapchainImages.data());

    // Create a good template for creating the image views
    VkImageViewCreateInfo imageView;
    memset(&imageView, 0, sizeof(VkImageViewCreateInfo));
    imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageView.format = vk::swapchainFormat;
    imageView.viewType = VK_IMAGE_VIEW_TYPE_2D;

    imageView.components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};
    imageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageView.subresourceRange.baseMipLevel = 0;
    imageView.subresourceRange.levelCount = 1;
    imageView.subresourceRange.baseArrayLayer = 0;
    imageView.subresourceRange.layerCount = 1;

    vk::swapchainImageViews.resize(vk::swapLength);

    // Create the image views
    for (uint32_t i = 0; i < vk::swapLength; i++) {
        imageView.image = vk::swapchainImages.at(i);

        if (vkCreateImageView(vk::logicalDevice, &imageView, nullptr, &vk::swapchainImageViews.at(i)) !=
            VK_SUCCESS) {
            Log.error("Could not retrieve swapchain image views");
            return false;
        }
    }

    // Send the swapchain size over to the camera buffer
    Camera::setProjection(vk::swapchainExtent.width, vk::swapchainExtent.height);
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
