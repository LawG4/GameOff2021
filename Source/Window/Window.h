/*!********************************************************************************************************
\File          : Window.h
\Copyright     : MIT License
\Brief         : Adds a header for window abstraction through GLFW
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include <GLFW/glfw3.h>

// Opaque handle to the GLFW window
extern GLFWwindow *window;

///<summary> Creates the GLFW window </summary>
///<returns>True if the window was created successfully </returns>
bool initWindow();