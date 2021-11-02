/*!********************************************************************************************************
\File          : Main.cpp
\Copyright     : MIT License
\Brief         : Adds an entry point into the game
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Log.h"
#include "Window.h"

#include <iostream>

int main(int argc, char *argv[])
{
    initLog();

    initWindow();
    std::cout << "Hello World" << std::endl;
}
