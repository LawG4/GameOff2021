/*!********************************************************************************************************
\File          : Camera.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the camera utilities
\Contributors  : Lawrence G
 *********************************************************************************************************/

#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "Gameplay.h"

void Camera::onWindowSize(GLFWwindow* window, int width, int height) { Gameplay::windowSize(width, height); }

using namespace Camera;

bool hasViewUpdated = true;
bool hasProjectionUpdated = true;

// Private camera variables
glm::vec3 cameraPosition = glm::vec3(0.0);
glm::mat4 viewProjectionMatrix = glm::identity<glm::mat4>();
glm::mat4 projectionMatrix = glm::identity<glm::mat4>();
glm::mat4 viewMatrix = glm::identity<glm::mat4>();

void Camera::setProjection(uint32_t width, uint32_t height)
{
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    // We want Y to go from -1 to 1
    // As a result X goes from -aspectRatio to aspectRatio
    projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, 1.0f, -1.0f);

    // Tell the user that the projection matrix has been updated
    hasProjectionUpdated = true;
}

bool Camera::getHasCameraUpdated() { return hasViewUpdated || hasProjectionUpdated; }

glm::mat4 Camera::getViewProjectionMatrix()
{
    // Has either of the underlying matrices changed?
    if (hasViewUpdated || hasProjectionUpdated) {
        viewProjectionMatrix = projectionMatrix * viewMatrix;

        // Tell inernals we won't have to recalculate until they are changed again
        hasViewUpdated = false;
        hasProjectionUpdated = false;
    }

    return viewProjectionMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
    // Always return the current projection matrix
    return projectionMatrix;
}

void Camera::setPosition(glm::vec3 position)
{
    // From the new position, set the new view matrix. Also tell the intenral structure that the view matrix
    // will need updating

    // viewMatrix = glm::lookAt(position, (position + glm::vec3(0, 0, 1)), {0, 1, 0});
    viewMatrix = glm::translate(glm::identity<glm::mat4>(), -position);

    hasViewUpdated = true;

    // store the camera position
    cameraPosition = position;
}

glm::vec3 Camera::getPosition() { return cameraPosition; }
