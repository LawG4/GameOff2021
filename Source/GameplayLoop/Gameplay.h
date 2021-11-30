/*!********************************************************************************************************
\File          : Gameplay.h
\Copyright     : GPL-3.0 License
\Brief         : Provides the gameplay loop so that main doesn't get too clustered
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Window.h"
#include "collision.h"

namespace Gameplay
{
/// <summary>Tells us if the gameplay is currently running or not</summary>
/// <returns>True if the game play loop is running</returns>
bool isActive();

/// <summary>Loads all of the main variables that the gameloop needs </summary>
void initialise();

/// <summary>Plays this frame of the gameplay loop</summary>
/// <param name="deltaTime">The frame time </param>
void playFrame(float deltaTime);

/// <summary> Collisions for front of hopper </summary>
bool frontcol();

/// <summary> Gameplay loop for actual game, entered from main </summary>
void gameLoop();

void windowSize(uint32_t width, uint32_t height);

void cleanup();

void randWallValue();
}  // namespace Gameplay

namespace Physics
{
/// <summary>Updates the players acceleration</summary>
/// <param name="acceleration">The acceleration to set</param>
void setAcceleration(glm::vec2 acceleration);

void setHorizontalAcceleration(float x);

/// <summary>Updates the players current velocity</summary>
/// <param name="velocity">The velocity to  set</param>
void setVelocity(glm::vec2 velocity);

glm::vec2 getVelocity();

/// <summary>Runs the physics calculations and returns the players position</summary>
/// <param name="deltaTime">Time that has passed</param>
/// <param name="rectangles">Bounding boxes for all the positions to test</param>
/// <returns>Players position</returns>
glm::vec2 updatePosition(double deltaTime, const std::vector<BoundingRect>& rectangles);

/// <summary>Make the grassshopper jump</summary>
void jump();
}  // namespace Physics
