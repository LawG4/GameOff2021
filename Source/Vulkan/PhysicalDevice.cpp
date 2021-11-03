/*!********************************************************************************************************
\File          : PhysicalDevice.cpp
\Copyright     : MIT License
\Brief         : Selects the best physical device
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"

#include <optional>

VkPhysicalDevice vk::physicalDevice = VK_NULL_HANDLE;
VkPhysicalDeviceProperties deviceProperties;
QueueFamilyIndices vk::selectedQueueFamilies;

std::vector<VkPhysicalDevice> devices;

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

bool QueueFamilyIndices::allQueuesPresent()
{
    return this->graphicsFamily.has_value() && this->presentFamily.has_value();
}

bool isDeviceSuitable(VkPhysicalDevice device)
{
    // Get the physical device properties
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    Log.info("Evaluating suitability for Vulkan device {}", deviceProperties.deviceName);

    // Does it have the suitable queues
    vk::selectedQueueFamilies = findQueueFamilies(device);
    if (!vk::selectedQueueFamilies.allQueuesPresent()) {
        Log.info("Rejecting physical device");
        return false;
    }

    // For now just assume it's good
    Log.info("Selected Physical device {}", deviceProperties.deviceName);
    return true;
}

bool vk::selectBestPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vk::instance, &deviceCount, nullptr);
    devices.resize(deviceCount);
    vkEnumeratePhysicalDevices(vk::instance, &deviceCount, devices.data());

    if (devices.size() == 0) {
        Log.error("Could not get a single vulkan compatible physical device");
        return false;
    }

    // For now just select the first device that iss appropriate
    for (auto device : devices) {
        if (isDeviceSuitable(device)) {
            vk::physicalDevice = device;
            break;
        }
    }

    // if there are still none suitable then break
    if (vk::physicalDevice == VK_NULL_HANDLE) {
        Log.error("Could not find a suitable physical device");
        return false;
    }

    return true;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    // Get the list of the current devices queues
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // Now loop through each one of the Queues looking for the graphics queue
    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        VkQueueFlags flags = queueFamilies.at(i).queueFlags;

        // Use the first queue that has graphics support
        if (flags & VK_QUEUE_GRAPHICS_BIT && !indices.graphicsFamily.has_value()) {
            indices.graphicsFamily = i;
        }

        // Does this queue have support for presentation?
        if (!indices.presentFamily.has_value()) {
            VkBool32 presentationSupported = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vk::surface, &presentationSupported);

            if (presentationSupported == VK_TRUE) indices.presentFamily = i;
        }
    }

    return indices;
}
