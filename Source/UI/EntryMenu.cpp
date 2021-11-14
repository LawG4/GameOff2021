/*!********************************************************************************************************
\File          : EntryMenu.cpp
\Copyright     : GPL-3.0 License
\Brief         : Creation of main menu
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "EntryMenu.h"
#include <fstream>
#include <iostream>
#include "Objects.h"
#include "nlohmann/json.hpp"

#include "Log.h"
#include "Render.h"
#include "Window.h"
#include "nlohmann/json.hpp"

#include "Cursor_input.h"
#include "EntryMenu.h"
#include "Keyboard_input.h"
#include "Player_object.h"

#include "Objects.h"

#include <fstream>
#include <iostream>

// Create a 2d square object (window size is vertex 3.5 x 2?)
EntryMenu::EntryMenu()
{
    // const std::vector<glm::vec3> pos = {{1.5f, -0.5f, 0.0f}, {1.5f, 0.5f, 0.0f}, {-1.5f, 0.5f, 0.0f}};
    std::vector<glm::vec3> pos2 = {{-1.5f, 0.5f, 0.0f}, {-1.5f, -0.5f, 0.0f}, {1.5f, -0.5f, 0.0f}};
    // col = {{5.0f, 1.0f, 0.0f}, {5.0f, 1.0f, 0.0f}, {5.0f, 1.0f, 0.0f}};
}
// Generate triangles
void EntryMenu::click_button()
{
    // RenderObject2D *Triangle =new RenderObject2D({{1.5f, -0.5f, 0.0f}, {1.5f, 0.5f, 0.0f}, {-1.5f, 0.5f,
    // 0.0f}}, {{-1.5f, 0.5f, 0.0f}, {-1.5f, -0.5f, 0.0f}, {1.5f, -0.5f, 0.0f}});
    RenderObject2D *Triangle2 = new RenderObject2D(EntryMenu::pos, EntryMenu::col);
}

// Build player class as GLOBAL
EntryMenu *MainMenu = new EntryMenu;
