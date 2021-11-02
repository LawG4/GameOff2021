/*!********************************************************************************************************
\File          : DisplayList.cpp
\Copyright     : MIT License
\Brief         : Holds the basic display list to render everything this frame
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Render.h"

void DisplayElement::drawGL() { Log.warn("Trying to draw default GL display list"); }
void DisplayElement::drawVulkan() { Log.warn("Trying to draw default Vulkan display list"); }

void renderListToWindow(GLFWwindow* context, DisplayList& displayList)
{
    // Check which API we're using on the current window
    if (windowUsingVulkan[context]) {
        // we're using Vulkan
        for (auto displayElement : displayList.elements) {
            displayElement.drawVulkan();
        }
    } else {
        // We're using GL
        for (auto displayElement : displayList.elements) {
            displayElement.drawGL();
        }
    }
}
