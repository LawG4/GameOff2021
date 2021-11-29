/*!********************************************************************************************************
\File          : PlayerPhysics.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the player object with physics, everything else is static
\Contributors  : Lawrence G, Freddie M
 *********************************************************************************************************/
#include "Gameplay.h"

#define TOP_SPEED (1000.0)
#define FLOOR_Y_POS (-2.6)
#define GRAVITY (-9.8)
// Players current acceleration
glm::vec2 _acceleration = glm::vec2(0);

// Player's current velocity
glm::vec2 _velocity = glm::vec2(0);

glm::vec2 _position = glm::vec2(0);

glm::vec2 _leftFootOffset = glm::vec2(0);
glm::vec2 _rightFootOffset = glm::vec2(0);

// Is the player currently falling?
bool _falling = true;

glm::vec2 Physics::updatePosition(double deltaTime, const std::vector<BoundingRect>& rectangles)
{
    // If the player is currently falling, then before we update the position we have to add the gravity
    if (_falling) _acceleration.y = GRAVITY;

    // Now using the delta time update the velocity
    _velocity += _acceleration * static_cast<float>(deltaTime);

    // Apply some friction to the player's velocity, if their acceleration is 0
    if (_acceleration.x == 0) _velocity.x -= 1.0f * deltaTime * _velocity.x;

    // clamp the velocity to a top speed
    if (glm::length(_velocity) >= TOP_SPEED) _velocity = glm::vec2(TOP_SPEED) * glm::normalize(_velocity);

    // Using the delta time and the new velocity update the position of the hopper
    _position += glm::vec2(deltaTime) * _velocity;

    // Has the player hit the floor? if so, we want to stop them
    if (_position.y < FLOOR_Y_POS) {
        _falling = false;           // not falling
        _position.y = FLOOR_Y_POS;  // pop them back to the floor
        _velocity.y = 0;

        // We can skip Y position calculations yeah yeah, I know goto sucks...
        goto HORIZONTAL_CALCS;
    }

HORIZONTAL_CALCS:

    return _position;
}

void Physics::jump()
{
    // Can't jump if we're falling
    if (_falling) return;

    _falling = true;
    _velocity.y = 10;

    // Begin using jump animations
}

void Physics::setVelocity(glm::vec2 velocity) {}

glm::vec2 Physics::getVelocity() { return _velocity; }

void Physics::setAcceleration(glm::vec2 acceleration) { _acceleration = acceleration; }

void Physics::setHorizontalAcceleration(float x) { _acceleration.x = x; }
