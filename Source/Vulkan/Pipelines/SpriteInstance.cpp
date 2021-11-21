/*!********************************************************************************************************
\File          : SpriteInstance.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides rendering for copies of sprites
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Sprites.h"

void SpriteInstance::render()
{
    // Call up to the sprite and render, while passing the mvp to translate it
    _sprite->render(getMvp());
}

SpriteInstance::SpriteInstance(Sprite* sprite)
{
    _sprite = sprite;
    _pos = glm::vec3(0, 0, 0);
    _scale = glm::vec3(1.0, 1.0, 0.0);
    _rot = glm::vec3(0.0, 0.0, 0.0);

    _mvp = calculateMVP();
    _mvpOutdated = false;
}

SpriteInstance::SpriteInstance(Sprite* sprite, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot)
{
    _sprite = sprite;
    _pos = pos;
    _scale = scale;
    _rot = rot;

    // Calculate the mvp matrix
    _mvp = calculateMVP();
    _mvpOutdated = false;
}

SpriteInstance::~SpriteInstance() {}

glm::mat4 SpriteInstance::getMvp()
{
    if (_mvpOutdated) {
        _mvp = calculateMVP();
        _mvpOutdated = false;
    }
    return _mvp;
}

glm::mat4 SpriteInstance::calculateMVP()
{
    glm::mat4 mat = glm::identity<glm::mat4>();

    mat = glm::mat4(glm::quat(_rot));
    mat = glm::scale(mat, _scale);
    mat = glm::translate(mat, _pos);
    mat = ProjectionMatrices::orthogonal * mat;

    return mat;
}

void SpriteInstance::setPosition(glm::vec3 position)
{
    _mvpOutdated = true;
    _pos = position;
}

void SpriteInstance::setRotation(glm::vec3 rotation)
{
    _mvpOutdated = true;
    _rot = rotation;
}

void SpriteInstance::setScale(glm::vec3 scale)
{
    _mvpOutdated = true;
    _scale = scale;
}

glm::vec3 SpriteInstance::getPosition() { return _pos; }
glm::vec3 SpriteInstance::getRotation() { return _rot; }
glm::vec3 SpriteInstance::getScale() { return _scale; }
