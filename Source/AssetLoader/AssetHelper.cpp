/*!********************************************************************************************************
\File          : AssetHelper.cpp
\Copyright     : GPL-3.0 License
\Brief         : Automates the asset loading
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "AssetHelper.h"

std::vector<glm::vec2> Textures::generateTexCoordinates(glm::vec2 topLeft, glm::vec2 spriteSize,
                                                        glm::vec2 textureSize)
{
    std::vector<glm::vec2> tex(4);

    // Find the texel size
    glm::vec2 texSize = glm::vec2(1.0, 1.0) / textureSize;

    return tex;
}
