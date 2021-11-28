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

/// <summary>
///  Check collision for hopper sprite
/// </summary>
/// <param name="x1_vertex">: X vertex</param>
/// <param name="x2_vertex">: Y vertex</param>
/// <param name="hopper_pos">: Hopper position in world</param>
/// <param name="move_direction">: Direction of movement (forwards = 1, backwards = 2, up = 3, down =
/// 4)</param>
bool collision_detection::check_collision_position(float x1_vertex, float y2_vertex, glm::vec3 hopper_pos,
                                                   int move_direction)
{
    // Convert positional data into cube mesh
    x1 = x1_vertex - 0.5;
    x2 = x1_vertex + 0.5;
    y1 = y2_vertex - 0.5;
    y2 = y2_vertex + 0.5;

    // Check to see if collision occurs in mesh
    if (move_direction == 1) {  // forward

        x1 = x1_vertex - 0.5;
        hopper_x1 = hopper_pos[0] + 0.5;
        if (x1 > hopper_x1) {
            return true;
        } else {
            return false;
        }

    } else if (move_direction == 2) {  // backwards

        x2 = x1_vertex + 0.5;
        hopper_x2 = hopper_pos[0] - 0.5;
        if (x2 > hopper_x2) {
            return true;
        } else {
            return false;
        }

    } else if (move_direction == 3) {  // up

        y2 = y2_vertex + 0.5;
        hopper_y2 = hopper_pos[2] + 0.5;
        if (y2 > hopper_y2) {
            return true;
        } else {
            return false;
        }

    } else if (move_direction == 4) {  // down

        y1 = y2_vertex - 0.5;
        hopper_y1 = hopper_pos[2] + 0.5;
        if (y2 > hopper_y1) {
            return true;
        } else {
            return false;
        }
    }

    if ((x1 > x_coordinate_range[0] && x2 < x_coordinate_range[1]) &&
        (y1 > y_coordinate_range[0] && y2 < y_coordinate_range[1])) {
        return true;
    } else {
        return false;
    }
}

// Initialise collisions object
collision_detection *collisions = new collision_detection;
