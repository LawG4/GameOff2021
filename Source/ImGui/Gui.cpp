/*!********************************************************************************************************
\File          : Gui.cpp
\Copyright     : MIT License
\Brief         : Adds a type of ui Rendering for ImGui
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Gui.h"
#include "Log.h"

void renderUI(GLFWwindow* context)
{
    if (context == launcher) {
        Log.info("Rendering ui");
    }
}
