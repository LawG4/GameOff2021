/*!********************************************************************************************************
\File          : Animation.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides animation by cycling sprites.
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Animation.h"
#include <cmath>
#include "Log.h"

AnimatedSprite::AnimatedSprite(SpriteSheet* sheet, const std::vector<std::vector<glm::vec2>>& texCoords,
                               double totalTime, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot)
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

    // Store the total time one loop of animastion takes
    _totalTime = totalTime;
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

void AnimatedSprite::render(glm::mat4 mvp)
{
    // Ensure that the sprite index is kept within bounds
    _spriteIndex = _spriteIndex % _spriteCount;

    // now pass the mvp over to the current frame for rendering
    _sprites[_spriteIndex]->render(mvp);
}

void AnimatedSprite::updateDelta(double delta)
{
    // Add to the internal clock, but insure it wraps around
    _currentTime = fmod(_currentTime + delta, _totalTime);

    // Now get the animation index
    _spriteIndex = static_cast<uint32_t>(static_cast<double>(_sprites.size()) * (_currentTime / _totalTime));
}

void AnimatedSprite::setTime(double time)
{
    // Add to the internal clock, but insure it wraps around
    _currentTime = time;

    // Now get the animation index
    _spriteIndex = static_cast<uint32_t>(static_cast<double>(_sprites.size()) * (_currentTime / _totalTime));
}

// Pass all of the positions and such over to the underlying instance

void AnimatedSprite::setPosition(glm::vec3 pos) { _instance->setPosition(pos); }

void AnimatedSprite::setRotation(glm::vec3 rot) { _instance->setRotation(rot); }

void AnimatedSprite::setScale(glm::vec3 scale) { _instance->setScale(scale); }

glm::vec3 AnimatedSprite::getPosition() { return _instance->getPosition(); }

glm::vec3 AnimatedSprite::getRotation() { return _instance->getRotation(); }

glm::vec3 AnimatedSprite::getScale() { return _instance->getScale(); }

// Populate the animation instance

AnimationInstance::AnimationInstance() {}
AnimationInstance::~AnimationInstance() {}
AnimationInstance::AnimationInstance(AnimatedSprite* animated, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot)
{
    _animated = animated;
    _pos = pos;
    _scale = scale;
    _rot = rot;
}

void AnimationInstance::render() { _animated->render(getMvp()); }
