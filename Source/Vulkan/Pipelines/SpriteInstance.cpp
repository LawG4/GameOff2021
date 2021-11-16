
#include "Sprites.h"

SpriteInstance::SpriteInstance(Sprite* sprite) {}

SpriteInstance::~SpriteInstance() {}

void SpriteInstance::setPosition(glm::vec3 position) {}
void SpriteInstance::setRotation(glm::vec3 rotation) {}
void SpriteInstance::setScale(glm::vec3 scale) {}

glm::mat4 SpriteInstance::getMvp() { return _mvp; }
glm::vec3 SpriteInstance::getPosition() { return _pos; }
glm::vec3 SpriteInstance::getRotation() { return _rot; }
glm::vec3 SpriteInstance::getScale() { return _scale; }
