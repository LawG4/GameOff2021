/*!********************************************************************************************************
\File          : Vulkan.h
\Copyright     : MIT License
\Brief         : Adds a header for using GL
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once
#include "Window.h"

/// <summary> A struct used to hold all of the information to start a GL display list call. ie program shaders
/// etc </summary>
typedef struct GLDisplayList_t {
} GLDisplayList;

/// <summary> Starts OpenGL </summary>
/// <returns> Returns true if GL init successeded </returns>
bool initGL();
