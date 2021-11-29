/*!********************************************************************************************************
\File          : collision.h
\Copyright     : GPL-3.0 License
\Brief         : collision functions
\Contributors  : Freddie M, Lawrence
 *********************************************************************************************************/
#pragma once

#include "EntryMenu.h"
#include "nlohmann/json.hpp"

struct BoundingRect {
    /// <summary>Top left (x,y) coordinates of the rectangle</summary>
    glm::vec2 topLeft;

    /// <summary>Width of the rectangle</summary>
    float width;

    /// <summary>Height of the rectangle</summary>
    float height;
};

namespace Collision
{
/// <summary>Calculates if a point is indside the given rectangle</summary>
/// <param name="point">(x,y) coordinates of the point being tested)</param>
/// <param name="rect">Rectangle being tested</param>
/// <returns>True if the point is inside the rectangle</returns>
bool pointInBox(glm::vec2 point, const BoundingRect& rect);

bool boxInBox(const BoundingRect& rect1, const BoundingRect& rect2);
}  // namespace Collision
