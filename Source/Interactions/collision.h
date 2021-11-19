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
    bool check_collision(float x1_vertex, float x2_vertex, float y1_vertex, float y2_vertex, double xpos,
                         double ypos);

    // variables
    float pixel_scale_X;
    float pixel_scale_Y;
    float vertexx_M;
    float vertexy_M;
    float x_coordinate_range[2];
    float y_coordinate_range[2];
};

// Declare collisions object
extern collision_detection* collisions;
