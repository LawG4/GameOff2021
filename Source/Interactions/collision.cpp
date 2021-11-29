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
        if (point.y < rect.topLeft.y && (point.y > rect.topLeft.y - rect.height)) {
            // We are inside the rectangle
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Collision::boxInBox(const BoundingRect& rect1, const BoundingRect& rect2)
{
    // Are the sides of the rectangle touching?
    if (rect1.topLeft.x + rect1.width > rect2.topLeft.x &&   // first right edge past second left edge
        rect1.topLeft.x < rect2.topLeft.x + rect2.width &&   // first left edge is passed second right
        rect1.topLeft.y > rect2.topLeft.y - rect2.height &&  // first top edge is past the bottom
        rect1.topLeft.y - rect1.height < rect2.topLeft.y     // first bottom is past second top
    ) {
        return true;
    } else {
        return false;
    }
}
