/*!********************************************************************************************************
\File          : Ui.h
\Copyright     : MIT License
\Brief         : Header for UI abstraction
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once

#include "Render.h"

#include <vector>
/// <summary>A struct for holding one element on the UI, one bounding box on screen </summary>
struct UiObject {
    // Bounding box of the UI element
    float topLeftx;
    float topLefty;
    float width;
    float height;

    uint32_t textureID;

    // Color
    float r, g, b;

    // Turn this UI element into a display object for rendering
    DisplayElement createGLDisplayElement();
    // Turn this UI element into a display object for rendering
    DisplayElement createVulkanDisplayElement();
    // Get the vertices for this draw call
    std::vector<glm::vec3> createVertexList();
};
