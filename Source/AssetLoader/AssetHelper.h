/*!********************************************************************************************************
\File          : AssetHelper.h
\Copyright     : GPL-3.0 License
\Brief         : Automates the asset loading
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once
#include "Animation.h"
namespace Textures
{
std::vector<glm::vec2> generateTexCoordinates(glm::vec2 topLeft, glm::vec2 spriteSize, glm::vec2 textureSize);
glm::vec3 getTexSize(glm::vec2 spriteSize);
}  // namespace Textures

namespace AnimatedSprites
{
std::pair<SpriteSheet*, AnimatedSprite*> spinningCoin();
std::pair<SpriteSheet*, AnimatedSprite*> hopperwalk();
}  // namespace AnimatedSprites

namespace BackgroundSprites
{
std::pair<SpriteSheet*, Sprite*> CityCentre();
}
