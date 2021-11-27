/*!********************************************************************************************************
\File          : Gameplay.h
\Copyright     : GPL-3.0 License
\Brief         : Provides the gameplay loop so that main doesn't get too clustered
\Contributors  : Lawrence G,
 *********************************************************************************************************/

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

/// <summary> Gameplay loop for actual game, entered from main </summary>
void gameLoop();

void cleanup();
}  // namespace Gameplay
