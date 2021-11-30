/*!********************************************************************************************************
\File          : Camera.h
\Copyright     : GPL-3.0 License
\Brief         : Provides the camera utilities
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once

#include "Window.h"
#include "glm/glm.hpp"
namespace Camera
{
/// <summary>Gets information about if the camera has updated </summary>
/// <returns>True if the camera has changed</returns>
bool getHasCameraUpdated();

/// <summary>Sets the internal projection matrix given the screen paramaters </summary>
/// <param name="width">width of the swapchain</param>
/// <param name="height">height of the swapchain</param>
void setProjection(uint32_t width, uint32_t height);

/// <summary>Gets the matrix that is responsible for flattening objects to the screen</summary>
/// <returns>Returns the projection matrix</returns>
glm::mat4 getProjectionMatrix();

/// <summary>Gets the matrix responsible for putting things into view space and flattening</summary>
/// <returns>Returns the view projection matrix</returns>
glm::mat4 getViewProjectionMatrix();

/// <summary>Sets the position for the camera</summary>
/// <param name="position">Position</param>
void setPosition(glm::vec3 position);

/// <summary>Get the current position of the camera</summary>
/// <returns>Returns the current position of the camera</returns>
glm::vec3 getPosition();

/// <summary>Callback to update all the things that need changing when the window size is updated</summary>
/// <param name="width">Width of window</param>
/// <param name="height">Height of Window</param>
void onWindowSize(GLFWwindow* window, int width, int height);

void scroll(double deltaTime);
}  // namespace Camera
