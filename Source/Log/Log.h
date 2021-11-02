/*!********************************************************************************************************
\File          : Log.h
\Copyright     : MIT License
\Brief         : Adds a header for initing the loging system
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once
#include "spdlog/spdlog.h"

// Log object for logging to the terminal and log file
extern spdlog::logger Log;

/// <summary>Starts the logging system using spdlog. Should be the absolute first thing called</summary>
/// <returns>True if successful </returns>
bool initLog();
