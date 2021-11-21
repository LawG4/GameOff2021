/*!********************************************************************************************************
\File          : collision.cpp
\Copyright     : GPL-3.0 License
\Brief         : collision functions
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "collision.h"

// Collision object, initiated at begining of main with data it needs

void collision_detection::intialise_object(uint32_t ww, uint32_t wh)
{
    // Create pixel scale to convert vertex coord to pixel position (to use with cursor data)
    pixel_scale_X = ww / vertdimen[0];
    pixel_scale_Y = wh / vertdimen[1];
    vertexx_M = vertdimen[0];
    vertexy_M = vertdimen[1];
}

bool collision_detection::check_collision(float x1_vertex, float x2_vertex, float y1_vertex, float y2_vertex,
                                          double xpos, double ypos)
{
    // Convert vertex data to pixles
    x_coordinate_range[0] = ((x1_vertex + vertexx_M) / 2) * pixel_scale_X;

    x_coordinate_range[1] = ((x2_vertex + vertexx_M) / 2) * pixel_scale_X;

    y_coordinate_range[0] = ((y1_vertex + vertexy_M) / 2) * pixel_scale_Y;

    y_coordinate_range[1] = ((y2_vertex + vertexy_M) / 2) * pixel_scale_Y;

    // If statements to asses collision

    if ((xpos > x_coordinate_range[0] && xpos < x_coordinate_range[1]) &&
        (ypos > y_coordinate_range[0] && ypos < y_coordinate_range[1])) {
        return true;
    } else {
        return false;
    }
}

// Initialise collisions object
collision_detection *collisions = new collision_detection;
