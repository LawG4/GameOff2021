/*!********************************************************************************************************
\File          : Render.h
\Copyright     : MIT License
\Brief         : Handels rendering the current frame
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Vulkan.h"
#include "Window.h"

#include "glm/glm.hpp"

#include <unordered_map>
#include <vector>

struct DisplayElement {
    std::vector<glm::vec3> vertices;

    void drawVulkan();
    void drawGL();
};

/// <summary> A display list groups elements to be drawn by the same shader or graphics pipeline </summary>
typedef struct DisplayList_t {
    const char *displayListName;
    VulkanDisplayList vulkan;

    std::vector<DisplayElement> elements;
} DisplayList;

// A quad Element that inherates from the default display element
struct QuadElement : DisplayElement {
};

void renderListToWindow(GLFWwindow *context, DisplayList &displayList);
