/*!********************************************************************************************************
\File          : PlayerPhysics.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the player object with physics, everything else is static
\Contributors  : Lawrence G, Freddie M
 *********************************************************************************************************/
#include "Gameplay.h"

#define TOP_SPEED (1000.0)
#define FLOOR_Y_POS (-0.8)
#define GRAVITY (-9.8)
#define FRICTION (1.1f)
// Players current acceleration
glm::vec2 _acceleration = glm::vec2(0);

// Player's current velocity
glm::vec2 _velocity = glm::vec2(0);

glm::vec2 _position = glm::vec2(0);

const glm::vec2 _spriteSize = glm::vec2(0.28, 0.10);
const glm::vec2 _spriteOffset = glm::vec2(-0.16, -0.04);

// Is the player currently falling?
bool _falling = true;

glm::vec2 Physics::updatePosition(double deltaTime, const std::vector<BoundingRect>& rectangles)
{
    // If the player is currently falling, then before we update the position we have to add the gravity
    if (_falling) _acceleration.y = GRAVITY;

    // Now using the delta time update the velocity
    _velocity += _acceleration * static_cast<float>(deltaTime);

    // Apply some friction to the player's velocity, if their acceleration is 0
    _velocity = _velocity / (1.0f + FRICTION * static_cast<float>(deltaTime));

    // clamp the velocity to a top speed
    if (glm::length(_velocity) >= TOP_SPEED) _velocity = glm::vec2(TOP_SPEED) * glm::normalize(_velocity);

    // Using the delta time and the new velocity update the position of the hopper
    _position += glm::vec2(deltaTime) * _velocity;

    // Has the player hit the floor? if so, we want to stop them
    if (_position.y < FLOOR_Y_POS) {
        _falling = false;           // not falling
        _position.y = FLOOR_Y_POS;  // pop them back to the floor
        _velocity.y = 0;
    }

    // Push the sprite coordinates into a bounding rectangle
    BoundingRect spriteRect{_position + _spriteOffset, _spriteSize.x, _spriteSize.y};

    // Take a copy of all boxes which collide in the Y direction
    std::vector<BoundingRect> reTest;

    // First test the Y direction collision and resolve
    for (const BoundingRect& rect : rectangles) {
        // We have collided with this box, push it out
        if (Collision::boxInBox(spriteRect, rect)) {
            // This box will need retesting in the x direction
            reTest.push_back(rect);

            // if the Y velocity is negative then we're falling onto something
            if (_velocity.y < 0) {
                // Stop falling
                _falling = false;
                _velocity.y = 0;

                // Push out
                spriteRect.topLeft.y = rect.topLeft.y + spriteRect.height;
            } else if (_velocity.y > 0) {
                // We smacked a cieling, so stay fallign
                _velocity.y = 0;
                // pushout
                spriteRect.topLeft.y = rect.topLeft.y - rect.height;
            }
        }
    }

    // Now retest any colisions that happened in the Y direction, this time resolving in the x direction
    for (const BoundingRect& rect : reTest) {
        if (Collision::boxInBox(spriteRect, rect)) {
            // There is still an x collision that needs resolving
            Log.info("Retest collision");

            // If the x direction is positive then push the hopper left
            if (_velocity.x > 0) {
                // Stop hopper from advancing
                _velocity.x = 0;
                // Push out
                spriteRect.topLeft.x = rect.topLeft.x - spriteRect.width;
            } else if (_velocity.x < 0) {
                // Hopper is moving left so push hopper right
                _velocity.x = 0;
                spriteRect.topLeft.x = rect.topLeft.x + rect.width;
            }
        }
    }

    // Set the position to what is in the rectangle
    _position = spriteRect.topLeft - _spriteOffset;

    return _position;
}

void Physics::jump()
{
    // Can't jump if we're falling
    if (_falling) return;

    _falling = true;
    _velocity.y = 4;
}

BoundingRect Physics::boxFromSprite(SpriteInstance& instance)
{
    // Get the top left position
    glm::vec2 topLeft = glm::vec2(instance.getPosition().x - 0.5f * instance.getScale().x,
                                  instance.getPosition().y + 0.5 * instance.getScale().y);
    return {topLeft, instance.getScale().x, instance.getScale().y};
}

void Physics::setVelocity(glm::vec2 velocity) {}

glm::vec2 Physics::getVelocity() { return _velocity; }

void Physics::setAcceleration(glm::vec2 acceleration) { _acceleration = acceleration; }

void Physics::setHorizontalAcceleration(float x) { _acceleration.x = x; }
