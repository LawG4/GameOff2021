/*!********************************************************************************************************
\File          : PhysicalDevice.cpp
\Copyright     : MIT License
\Brief         : Selects the best physical device
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"

VkPhysicalDevice vk::physicalDevice = VK_NULL_HANDLE;
VkPhysicalDeviceProperties deviceProperties;

std::vector<VkPhysicalDevice> devices;

bool isDeviceSuitable(VkPhysicalDevice device)
{
    // Get the physical device properties
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    Log.info("Evaluating suitability for Vulkan device {}", deviceProperties.deviceName);

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
