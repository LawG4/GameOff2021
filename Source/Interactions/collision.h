/*!********************************************************************************************************
\File          : collision.h
\Copyright     : GPL-3.0 License
\Brief         : collision functions
\Contributors  : Freddie M
 *********************************************************************************************************/
#pragma once

#include "EntryMenu.h"
#include "nlohmann/json.hpp"

// Collision object
class collision_detection
{
   public:
    // methods
    void intialise_object(uint32_t ww, uint32_t wh);

    // check_collision(Left X, Right X, Top Y, Bottom Y)
    bool check_collision(float x1_vertex, float x2_vertex, float y1_vertex, float y2_vertex, double xpos,
                         double ypos);

    bool check_collision_position(float x1_vertex, float y1_vertex, glm::vec3 hopper_pos, int move_direction);

    // variables
    float pixel_scale_X;
    float pixel_scale_Y;
    float vertexx_M;
    float vertexy_M;
    float x_coordinate_range[2];
    float y_coordinate_range[2];
    float x1 = 0;
    float x2 = 0;
    float y1 = 0;
    float y2 = 0;
    float hopper_x1 = 0;
    float hopper_x2 = 0;
    float hopper_y1 = 0;
    float hopper_y2 = 0;
};

// Declare collisions object
extern collision_detection* collisions;
