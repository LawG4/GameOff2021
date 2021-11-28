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

    // Find the top left, remember to centre the pixel
    topLeft = (topLeft * texSize);

    // fill in the texture coordinates starting from the bottom left and go counter clockwise
    spriteSize = spriteSize * texSize;
    tex[0] = topLeft + glm::vec2(0, 1) * spriteSize;
    tex[1] = topLeft + glm::vec2(1, 1) * spriteSize;
    tex[2] = topLeft + glm::vec2(1, 0) * spriteSize;
    tex[3] = topLeft + glm::vec2(0, 0) * spriteSize;

    return tex;
}

std::pair<SpriteSheet*, AnimatedSprite*> AnimatedSprites::spinningCoin()
{
    // create the pair
    std::pair<SpriteSheet*, AnimatedSprite*> pair;
    SpriteSheet* sheet = new SpriteSheet("Textures/SpinningCoin.png");

    // Using the tex coord generator get all of the keyframes of the animation
    std::vector<std::vector<glm::vec2>> tex(6);
    tex[0] = Textures::generateTexCoordinates({0, 0}, {28, 25}, {128, 128});
    tex[1] = Textures::generateTexCoordinates({0, 25}, {28, 25}, {128, 128});
    tex[2] = Textures::generateTexCoordinates({0, 50}, {28, 25}, {128, 128});
    tex[3] = Textures::generateTexCoordinates({0, 75}, {28, 25}, {128, 128});
    tex[4] = Textures::generateTexCoordinates({0, 100}, {28, 25}, {128, 128});
    tex[5] = Textures::generateTexCoordinates({29, 25}, {28, 25}, {128, 128});

    AnimatedSprite* sprite = new AnimatedSprite(sheet, tex, 0.6);

    pair.first = sheet;
    pair.second = sprite;

    return pair;
}
