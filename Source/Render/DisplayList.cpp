/*!********************************************************************************************************
\File          : DisplayList.cpp
\Copyright     : GPL-3.0 License
\Brief         : Holds the basic display list to render everything this frame
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Render.h"

void DisplayElement::drawGL() { Log.warn("Trying to draw default GL display list"); }
void DisplayElement::drawVulkan() { Log.warn("Trying to draw default Vulkan display list"); }

void renderListToWindow(GLFWwindow* context, DisplayList& displayList) {}
