/*!********************************************************************************************************
\File          : Render.h
\Copyright     : MIT License
\Brief         : Handels rendering the current frame
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Window.h"

#include <unordered_map>

extern std::unordered_map<GLFWwindow *, bool> windowUsingVulkan;

typedef struct DisplayList_t {
} DisplayList;


void renderListToWindow(GLFWwindow *context, DisplayList displayList);

void setClearColor(GLFWwindow *context, float r, float g, float b);

void clearFrame(GLFWwindow *context);