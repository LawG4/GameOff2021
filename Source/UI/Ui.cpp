/*!********************************************************************************************************
\File          : Ui.h
\Copyright     : MIT License
\Brief         : UI abstraction
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Ui.h"

std::vector<glm::vec3> UiObject::createVertexList()
{
    // The GLFW mouse coordinates are relative to top left, like most image drawing things
    // convert those into GL screenspace elements

    float screenspaceX = this->topLeftx * 2.0 - 1.0;
    float screenspaceY = this->topLefty * 2.0 - 1.0;
    glm::vec3 topLeft = glm::vec3(screenspaceX, screenspaceY, 0.0);

    float screenWidth = 2.0 * this->width;
    float screenHeight = 2.0 * this->height;

    std::vector<glm::vec3> vecs;

    // Do a clockwise winding
    vecs.push_back(topLeft);
    vecs.push_back(topLeft + glm::vec3(screenWidth, 0.0, 0.0));
    vecs.push_back(topLeft + glm::vec3(screenWidth, screenHeight, 0.0));
    vecs.push_back(topLeft + glm::vec3(0.0, screenHeight, 0.0));

    return vecs;
}

DisplayElement UiObject::createGLDisplayElement()
{
    DisplayElement de;
    return de;
}

DisplayElement UiObject::createVulkanDisplayElement()
{
    DisplayElement de;
    return de;
}
