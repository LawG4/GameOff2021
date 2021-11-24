/*!********************************************************************************************************
\File          : Timer.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides utility for frame times for animation and physics
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Timer.h"
#include "Window.h"

bool _timePaused = false;

double _deltaTime = 0;
double _prevTime = 0;

void Time::startFrameTime()
{
    _prevTime = glfwGetTime();
    _timePaused = false;
}

void Time::EndFrameTime()
{
    // This is where the delta time is calculated. if the clock is paused then set delta time to 0
    if (_timePaused) {
        _deltaTime = 0;
        _timePaused = false;
    } else {
        double currentTime = glfwGetTime();
        _deltaTime = currentTime - _prevTime;
    }
}

void Time::pauseTime()
{
    // Since the frame timer either starts with this call or startFrameTime
    // We still need to store the previous time
    _prevTime = glfwGetTime();

    // Set paised to true
    _timePaused = true;
}

double Time::getDetlaTime() { return _deltaTime; }
