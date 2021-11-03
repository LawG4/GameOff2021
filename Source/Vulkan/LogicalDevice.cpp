/*!********************************************************************************************************
\File          : PhysicalDevice.cpp
\Copyright     : MIT License
\Brief         : Creates a logical device from the selected physical device
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"

VkDevice vk::logialDevice;
VkQueue vk::graphicsQueue;

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
}

void createDeviceQueueInfos(std::vector<VkDeviceQueueCreateInfo>& queues)
{
    // Queue prioarties are all set to one
    float queuePrioraty = 1.0;

    // Create a graphics queue
    VkDeviceQueueCreateInfo graphics;
    memset(&graphics, 0, sizeof(VkDeviceQueueCreateInfo));
    graphics.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphics.queueFamilyIndex = vk::selectedQueueFamilies.graphicsFamily.value();
    graphics.queueCount = 1;
    graphics.pQueuePriorities = &queuePrioraty;

    queues.push_back(graphics);
}

VkPhysicalDeviceFeatures selectDeviceFeatures()
{
    VkPhysicalDeviceFeatures features;
    memset(&features, 0, sizeof(VkPhysicalDeviceFeatures));
    return features;
}
