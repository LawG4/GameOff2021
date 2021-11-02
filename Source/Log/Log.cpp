/*!********************************************************************************************************
\File          : Log.cpp
\Copyright     : MIT License
\Brief         : Adds an implementation for initing the loging system
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Log.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

// If the user is compiling for release mode we need to add the windows header, that way we can close the
// console for release builds
#if defined(WIN32) && defined(NDEBUG)
#include <Windows.h>
#define FORCE_CLOSE_WIN32_CONSOLE 1
#endif  // defined(WIN32) && defined(NDEBUG)

spdlog::logger Log("Uninitialised Log");

bool initLog()
{
    // Absolutley first thing we should do is close the console in release configs
#ifdef FORCE_CLOSE_WIN32_CONSOLE
    FreeConsole();
#endif  // FORCE_CLOSE_WIN32_CONSOLE

    // Next we know that we will be setting up a file console to log everything
    auto fileLog = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true);
    fileLog->set_level(spdlog::level::trace);

    // Next if we're not on debug mode then make a console logger
#ifndef NDEBUF
    auto consoleLog = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleLog->set_level(spdlog::level::warn);

    spdlog::logger multi("Multi_Logger", {consoleLog, fileLog});
    Log = multi;
#else
    Log = fileLog;
#endif

    // Now set the log level warning for the multi log
    Log.set_level(spdlog::level::debug);

    Log.warn("Ooooh yummy log message");

    return true;
}
