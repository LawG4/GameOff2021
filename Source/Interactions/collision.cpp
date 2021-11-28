/*!********************************************************************************************************
\File          : collision.cpp
\Copyright     : GPL-3.0 License
\Brief         : collision functions
\Contributors  : Freddie M, Lawrence
 *********************************************************************************************************/

#include "collision.h"

bool Collision::pointInBox(glm::vec2 point, const BoundingRect& rect)
{
    // Is the point contained in the space bounded by the left and right sides of the rect
    if (point.x > rect.topLeft.x && point.x < rect.topLeft.x + rect.width) {
        // Is the point bounded by the space of the top and bottom of the rectangle
        if (point.y < rect.topLeft.y && point.y > rect.topLeft.y - rect.height) {
            // We are inside the rectangle
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
