/*!********************************************************************************************************
\File          : Render.h
\Copyright     : MIT License
\Brief         : Handels rendering the current frame
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "GL.h"
#include "Vulkan.h"
#include "Window.h"

#include "glm/glm.hpp"

#include <unordered_map>
#include <vector>

extern std::unordered_map<GLFWwindow *, bool> windowUsingVulkan;

struct DisplayElement {
    std::vector<glm::vec3> vertices;

    void drawVulkan();
    void drawGL();
};

/// <summary> A display list groups elements to be drawn by the same shader or graphics pipeline </summary>
typedef struct DisplayList_t {
    GLDisplayList gl;
    VulkanDisplayList vulkan;

    std::vector<DisplayElement> elements;
} DisplayList;

// A quad Element that inherates from the default display element
struct QuadElement : DisplayElement {
};

void renderListToWindow(GLFWwindow *context, DisplayList &displayList);

void setClearColor(GLFWwindow *context, float r, float g, float b);

void clearFrame(GLFWwindow *context);
