/*!********************************************************************************************************
\File          : PhysicalDevice.cpp
\Copyright     : GPL-3.0 License
\Brief         : Creates a logical device from the selected physical device
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"

#include <set>

VkDevice vk::logialDevice;
VkQueue vk::graphicsQueue;
VkQueue vk::presentationQueue;

// The device extensions we require
std::vector<const char*> vk::requiredDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
// Device extensions that we'd like to have but not fatal.
std::vector<const char*> vk::requestedDeviceExtensions = {};

/// <summary> Fetch the queues from the created physical device </summary>
void fetchQueues();

/// <summary> Fills a vector of queue device create infos</summary>
/// <param name="queues">The vector to fill</param>
void createDeviceQueueInfos(std::vector<VkDeviceQueueCreateInfo>& queues);

/// <summary> Selects the physical device features that we want to use </summary>
/// <returns>The struct of physical device features</returns>
VkPhysicalDeviceFeatures selectDeviceFeatures();

bool vk::createLogicalDevice()
{
    // First check if the physical device was actually created
    if (vk::physicalDevice == VK_NULL_HANDLE) {
        Log.error("Physical device hasn't been selected for logical device");
        return false;
    }

    // Create info for the device
    VkDeviceCreateInfo device;
    memset(&device, 0, sizeof(VkDeviceCreateInfo));
    device.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    // Add the queues to the logical device
    std::vector<VkDeviceQueueCreateInfo> queues;
    createDeviceQueueInfos(queues);
    device.pQueueCreateInfos = queues.data();
    device.queueCreateInfoCount = queues.size();

    // add the device extensions
    device.ppEnabledExtensionNames = vk::requiredDeviceExtensions.data();
    device.enabledExtensionCount = vk::requiredDeviceExtensions.size();

    // attach device features to the device create info
    device.pEnabledFeatures = &selectDeviceFeatures();

    if (vkCreateDevice(vk::physicalDevice, &device, nullptr, &vk::logialDevice) != VK_SUCCESS) {
        Log.error("Failed to created logical device");
        return false;
    }

    // Finally retireve the queues from the logical device
    fetchQueues();

    return true;
}

void fetchQueues()
{
    vkGetDeviceQueue(vk::logialDevice, vk::selectedQueueFamilies.graphicsFamily.value(), 0,
                     &vk::graphicsQueue);
    vkGetDeviceQueue(vk::logialDevice, vk::selectedQueueFamilies.presentFamily.value(), 0,
                     &vk::presentationQueue);
}

void createDeviceQueueInfos(std::vector<VkDeviceQueueCreateInfo>& queues)
{
    // We only want to create one queue per index, ie if graphics and presentation queue have the same value,
    // then we only want one queue create info
    // We can make the process of finding out the unique queue family indices easier using sets, as they only
    // allow unique values
    std::set<uint32_t> uniqueQueueIndicies = {vk::selectedQueueFamilies.graphicsFamily.value(),
                                              vk::selectedQueueFamilies.presentFamily.value()};

    // Queue prioarties are all set to one
    float queuePrioraty = 1.0;

    // For each unique family index create one device queue create info
    for (uint32_t queueIndex : uniqueQueueIndicies) {
        VkDeviceQueueCreateInfo queue;
        memset(&queue, 0, sizeof(VkDeviceQueueCreateInfo));
        queue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue.queueFamilyIndex = queueIndex;
        queue.queueCount = 1;
        queue.pQueuePriorities = &queuePrioraty;

        queues.push_back(queue);
    }

    // Create a graphics queue
    VkDeviceQueueCreateInfo graphics;
    memset(&graphics, 0, sizeof(VkDeviceQueueCreateInfo));
    graphics.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphics.queueFamilyIndex = vk::selectedQueueFamilies.graphicsFamily.value();
    graphics.queueCount = 1;
    graphics.pQueuePriorities = &queuePrioraty;

    // queues.push_back(graphics);
}

VkPhysicalDeviceFeatures selectDeviceFeatures()
{
    VkPhysicalDeviceFeatures features;
    memset(&features, 0, sizeof(VkPhysicalDeviceFeatures));
    return features;
}
