/*!********************************************************************************************************
\File          : Timer.h
\Copyright     : GPL-3.0 License
\Brief         : Provides utility for frame times for animation and physics
\Contributors  : Lawrence G,
 *********************************************************************************************************/

namespace Time
{

/// <summary>Take the current time for the start frame</summary>
void startFrameTime();

/// <summary>Take the current time for the end of the frame and store the difference as the delta time
/// </summary>
void EndFrameTime();

/// <summary>Pause the current framer, so the next frame will be 0</summary>
void pauseTime();

/// <summary>Gets the length of time the previous frame took</summary>
/// <returns>Delta time</returns>
double getDetlaTime();
}  // namespace Time
