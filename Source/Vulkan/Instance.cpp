/*!********************************************************************************************************
\File          : Instance.cpp
\Copyright     : MIT License
\Brief         : Creates a Vulkan instance
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"
#include "Window.h"

VkInstance vk::instance;

bool vk::createInstance()
{
    Log.info("Creating Vulkan instance");

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

    // Use glfw to get a minimal required instance extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    // Attach extensions to instance
    instance.enabledExtensionCount = glfwExtensionCount;
    instance.ppEnabledExtensionNames = glfwExtensions;

    // No instance layers loaded in
    instance.enabledLayerCount = 0;

    if (vkCreateInstance(&instance, nullptr, &vk::instance) != VK_SUCCESS) {
        return false;
    }
    return true;
}
