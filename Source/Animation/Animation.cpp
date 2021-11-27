/*!********************************************************************************************************
\File          : Animation.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides animation by cycling sprites.
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Animation.h"
#include "Log.h"

AnimatedSprite::AnimatedSprite(SpriteSheet* sheet, const std::vector<std::vector<glm::vec2>>& texCoords,
                               float timeLength, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot)
{
    // Ensure that there is atleast one tex coord
    if (texCoords.size() < 1) {
        Log.error("Not enough texture coordinates passed to animation sprite");
        return;
    }

    // Create each of the sprites
    _sprites.resize(texCoords.size());
    for (uint32_t i = 0; i < texCoords.size(); i++) {
        _sprites[i] = new Sprite(sheet, texCoords.at(i));
    }

    // Store the sprite count
    _spriteCount = texCoords.size();

    // To manage the underside of the positions and model project stuff, we use one underlying sprite instance
    _instance = new SpriteInstance(_sprites[0], pos, scale, rot);
}

AnimatedSprite::~AnimatedSprite()
{
    // Delete the underlying instance
    if (_instance) delete _instance;

    // Delete each of the underlying sprites
    for (Sprite* sprite : _sprites) {
        if (sprite) delete sprite;
    }

    // We're not managing the sprite sheet
}

void AnimatedSprite::render()
{
    // Ensure that the sprite index is kept in bounds
    _spriteIndex = _spriteIndex % _spriteCount;

    // Now make a render call by using the sprite directly instead of the sprite instance
    // We can use the underlying instance to insure that the animations are in the same place
    _sprites[_spriteIndex]->render(_instance->getMvp());
}

void AnimatedSprite::updateDelta(double delta) {}

void AnimatedSprite::setTime(double time) {}

// Pass all of the positions and such over to the underlying instance

void AnimatedSprite::setPosition(glm::vec3 pos) { _instance->setPosition(pos); }

void AnimatedSprite::setRotation(glm::vec3 rot) { _instance->setRotation(rot); }

void AnimatedSprite::setScale(glm::vec3 scale) { _instance->setScale(scale); }

glm::vec3 AnimatedSprite::getPosition() { return _instance->getPosition(); }

glm::vec3 AnimatedSprite::getRotation() { return _instance->getRotation(); }

glm::vec3 AnimatedSprite::getScale() { return _instance->getScale(); }
