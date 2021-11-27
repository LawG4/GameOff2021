/*!********************************************************************************************************
\File          : Animation.h
\Copyright     : GPL-3.0 License
\Brief         : Provides animation by cycling sprites.
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once
#include "Sprites.h"

class AnimatedSprite
{
   public:
    AnimatedSprite(SpriteSheet* sheet, const std::vector<std::vector<glm::vec2>>& texCoords, float timeLength,
                   glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0),
                   glm::vec3 rot = glm::vec3(0.0, 0.0, 0.0));
    ~AnimatedSprite();

    /// <summary>Renders the currently selected frame </summary>
    void render();

    void updateDelta(double delta);

    void setTime(double time);

    void setPosition(glm::vec3 pos);

    void setRotation(glm::vec3 rot);

    void setScale(glm::vec3 scale);

    glm::vec3 getPosition();

    glm::vec3 getRotation();

    glm::vec3 getScale();

   private:
    SpriteSheet* _sheet = nullptr;

    std::vector<Sprite*> _sprites;

    SpriteInstance* _instance = nullptr;

    uint32_t _spriteIndex = 0;

    uint32_t _spriteCount = 0;

    double _currentTime = 0.0;
};
