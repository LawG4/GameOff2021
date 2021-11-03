/*!********************************************************************************************************
\File          : Instance.cpp
\Copyright     : MIT License
\Brief         : Creates a Vulkan instance
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"
#include "Window.h"

VkInstance vk::instance;
std::vector<const char*> vk::validatedRequestedInstanceExtensions;
std::vector<VkExtensionProperties> vk::instanceExtensionProperties;

/// <summary> Checks that all instance extensions requested are supported and if not remove them from the list
/// </summary> <returns>A vector of actually enabled extensions</returns>
std::vector<const char*> validatedInstanceExtensions(std::vector<const char*>& requestedInstanceExtensions)
{
    std::vector<const char*> supportedExtensions;

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    // Reserve space in our vector
    vk::instanceExtensionProperties.resize(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, vk::instanceExtensionProperties.data());

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

bool vk::createInstance()
{
    VkApplicationInfo app;
    memset(&app, 0, sizeof(app));
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pApplicationName = "B.U.G Game off 2021";
    app.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app.pEngineName = "Custom Engine";
    app.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instance;
    memset(&instance, 0, sizeof(instance));
    instance.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance.pApplicationInfo = &app;

    // Get which instance extemsions the user would like to enable
    std::vector<const char*> requestedInstanceExtensions;

    // Use glfw to get a minimal required instance extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        requestedInstanceExtensions.push_back(glfwExtensions[i]);
    }

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

    // No instance layers loaded in
    instance.enabledLayerCount = 0;

    if (vkCreateInstance(&instance, nullptr, &vk::instance) != VK_SUCCESS) {
        return false;
    }
    return true;
}
