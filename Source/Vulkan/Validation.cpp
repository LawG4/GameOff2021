/*!********************************************************************************************************
\File          : Vulkan.h
\Copyright     : GPL-3.0 License
\Brief         : Adds vulkan validation layer support
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"

#ifdef NDEBUG
bool vk::validationLayersEnabled = false;
bool validationLayersRequested = false;
#else
// until proven otherwise
bool vk::validationLayersEnabled = false;
bool validationLayersRequested = true;
#endif  // NDEBUG

// Name of the validation layer
const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

VkDebugUtilsMessengerEXT vk::messenger;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessenger(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        Log.error("Validation Error {}", pCallbackData->pMessage);
    } else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        Log.warn("Validation Warning {}", pCallbackData->pMessage);
    }
    return VK_FALSE;
}

void populateDebugMessageCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    memset(&createInfo, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));

    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugMessenger;
}

void vk::addValidationLayersAndExtensions(std::vector<const char*>& extensions,
                                          std::vector<const char*>& layers)
{
    if (!validationLayersRequested) return;

    // Iterate through all the layers and see if the validation layer exists
    for (const auto& layer : validationLayers) {
        bool layerFound = false;
        for (const auto& layerProperties : vk::instanceLayerProperties) {
            if (!strcmp(layer, layerProperties.layerName)) {
                layerFound = true;
                layers.push_back(layer);
                break;
            }
        }

        if (!layerFound) {
            Log.warn("Instance layer {} not found", layer);
            vk::validationLayersEnabled = false;
            return;
        }
    }

    // We got here so the validation layers must exist
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    vk::validationLayersEnabled = true;
    Log.info("Enabling Valdiation layers");
}

void vk::createDebugMessenger()
{
    // retrieve the function first
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vk::instance,
                                                                          "vkCreateDebugUtilsMessengerEXT");
    if (!func) {
        Log.error("Could not retireve function pointer for creating a Vulkan debug messenger");
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessageCreateInfo(createInfo);
    if (func(vk::instance, &createInfo, nullptr, &vk::messenger) != VK_SUCCESS) {
        Log.error("Could not create a debug messenger callback");
    } else {
        Log.info("Created Vulkan debug messenger callback");
    }
}

void vk::deleteDebugMessenger()
{
    // retrieve the function
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vk::instance,
                                                                           "vkDestroyDebugUtilsMessengerEXT");
    if (!func) {
        Log.error("Could not load function pointer to destroy vulkan callback messenger");
        return;
    }

    func(vk::instance, vk::messenger, nullptr);
    Log.info("Vulkan Debug messenger destroyed");
}
