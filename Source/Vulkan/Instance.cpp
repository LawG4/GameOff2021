/*!********************************************************************************************************
\File          : Instance.cpp
\Copyright     : GPL-3.0 License
\Brief         : Creates a Vulkan instance
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"
#include "Window.h"

VkInstance vk::instance;
std::vector<const char*> vk::validatedRequestedInstanceExtensions;
std::vector<VkExtensionProperties> vk::instanceExtensionProperties;
std::vector<const char*> vk::validatedRequestedInstanceLayers;
std::vector<VkLayerProperties> vk::instanceLayerProperties;

/// <summary> Checks that all instance extensions requested are supported and if not remove them from the list
/// </summary> <returns>A vector of actually enabled extensions</returns>
std::vector<const char*> validatedInstanceExtensions(std::vector<const char*>& requestedInstanceExtensions)
{
    std::vector<const char*> supportedExtensions;

    // Go through each of the requested instance extensions, if they are unavailble then inform the user
    for (auto extension : requestedInstanceExtensions) {
        bool found = false;
        for (auto& knownExtension : vk::instanceExtensionProperties) {
            if (!strcmp(knownExtension.extensionName, extension)) {
                found = true;
                break;
            }
        }

        if (!found) {
            Log.warn("Could not find instance extension {}", extension);
        } else {
            supportedExtensions.push_back(extension);
        }
    }
    return supportedExtensions;
}

/// <summary> Validates that all of the layers requested are usable </summary>
/// <param name="requestedInstanceLayers">The layers being requested</param>
/// <returns>A list of supported layers</returns>
std::vector<const char*> validatedInstanceLayers(std::vector<const char*>& requestedInstanceLayers)
{
    std::vector<const char*> supportedLayers;

    for (const auto& requestedLayer : requestedInstanceLayers) {
        bool found = false;
        for (const auto& knownLayer : vk::instanceLayerProperties) {
            if (!strcmp(requestedLayer, knownLayer.layerName)) {
                found = true;
                break;
            }
        }

        if (!found) {
            Log.warn("Could not find instance layer {}", requestedLayer);
        } else {
            supportedLayers.push_back(requestedLayer);
        }
    }

    return supportedLayers;
}

bool vk::createInstance()
{
    // We'll need the instance extension and instance layer properties to be availble globally
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    vk::instanceExtensionProperties.resize(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, vk::instanceExtensionProperties.data());

    uint32_t propCount = 0;
    vkEnumerateInstanceLayerProperties(&propCount, nullptr);
    vk::instanceLayerProperties.resize(propCount);
    vkEnumerateInstanceLayerProperties(&propCount, vk::instanceLayerProperties.data());

    VkApplicationInfo app;
    memset(&app, 0, sizeof(app));
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pApplicationName = "B.U.G Game off 2021";
    app.applicationVersion = VK_MAKE_VERSION(1, 1, 0);
    app.pEngineName = "Custom Engine";
    app.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instance;
    memset(&instance, 0, sizeof(instance));
    instance.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance.pApplicationInfo = &app;

    // Get which instance extensions the user would like to enable
    std::vector<const char*> requestedInstanceExtensions;

    // Use glfw to get a minimal required instance extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        requestedInstanceExtensions.push_back(glfwExtensions[i]);
    }

    // Add validation layers and or extensions
    std::vector<const char*> requestedInstanceLayers;
    vk::addValidationLayersAndExtensions(requestedInstanceExtensions, requestedInstanceLayers);

    // Validate the extensions
    vk::validatedRequestedInstanceExtensions = validatedInstanceExtensions(requestedInstanceExtensions);
    if (vk::validatedRequestedInstanceExtensions.size() == requestedInstanceExtensions.size()) {
        Log.info("All requested instance extensions supported");
    } else {
        Log.info("Not all requested instance extensions are supported");
    }

    // Attach extensions to instance
    instance.enabledExtensionCount = vk::validatedRequestedInstanceExtensions.size();
    instance.ppEnabledExtensionNames = vk::validatedRequestedInstanceExtensions.data();

    // Validate the layers
    vk::validatedRequestedInstanceLayers = validatedInstanceLayers(requestedInstanceLayers);
    if (vk::validatedRequestedInstanceLayers.size() == requestedInstanceLayers.size()) {
        Log.info("All instance layers supported");
    } else {
        Log.warn("Not all instance layers are supported");
    }

    // Attach layers to the instance
    instance.enabledLayerCount = vk::validatedRequestedInstanceLayers.size();
    instance.ppEnabledLayerNames = vk::validatedRequestedInstanceLayers.data();

    if (vkCreateInstance(&instance, nullptr, &vk::instance) != VK_SUCCESS) {
        return false;
    }

    if (vk::validationLayersEnabled) {
        vk::createDebugMessenger();
    }

    return true;
}
