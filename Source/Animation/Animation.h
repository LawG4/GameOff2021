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
    /// <summary>Creates an animated sprite index, it owns all it's sprites but not the sprite sheet</summary>
    /// <param name="sheet">Pointer to the sheet containing all the animation frames</param>
    /// <param name="texCoords">A list of texture coordinates of each of the animation frames</param>
    /// <param name="totalTime">How long the animation takes to loop through</param>
    /// <param name="pos">Position of the sprite</param>
    /// <param name="scale">Scale of the sprite</param>
    /// <param name="rot">rotation of the sprite</param>
    AnimatedSprite(SpriteSheet* sheet, const std::vector<std::vector<glm::vec2>>& texCoords, double totalTime,
                   glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0),
                   glm::vec3 rot = glm::vec3(0.0, 0.0, 0.0));

    ~AnimatedSprite();

    /// <summary>Renders the currently selected frame </summary>
    void render();

    /// <summary>Renders an instance of this animated sprite for the currently selected frame at a position on
    /// screen</summary> <param name="mvp">The model view projection matrix </param>
    void render(glm::mat4 mvp);

    /// <summary>Updates how much time has passed from the previous frame</summary>
    /// <param name="delta">Time which has passed</param>
    void updateDelta(double delta);

    /// <summary>Set the internal timer of this animation clock</summary>
    /// <param name="time">The internal time to be set</param>
    void setTime(double time);

    /// <summary>Sets the position of the sprite</summary>
    /// <param name="pos">Position to set</param>
    void setPosition(glm::vec3 pos);

    /// <summary>Sets the rotation of the sprite</summary>
    /// <param name="rot">rotation to be set</param>
    void setRotation(glm::vec3 rot);

    /// <summary>Sets the scale of the sprite</summary>
    /// <param name="scale">scale to be set</param>
    void setScale(glm::vec3 scale);

    /// <summary>Gets the stored position of the sprite</summary>
    /// <returns>position</returns>
    glm::vec3 getPosition();

    /// <summary>Gets the stored rotation of the sprite</summary>
    /// <returns>rotation</returns>
    glm::vec3 getRotation();

    /// <summary>Gets the scale of the sprite</summary>
    /// <returns>scale</returns>
    glm::vec3 getScale();

   private:
    /// <summary>Internal reference to the sprite sheet being used</summary>
    SpriteSheet* _sheet = nullptr;

    /// <summary>List of all the frames of animation, reprsented by a series of sprites</summary>
    std::vector<Sprite*> _sprites;

    /// <summary>One underlying sprite instance, it just handles the modesl position for us</summary>
    SpriteInstance* _instance = nullptr;

    /// <summary>Current frame being rendered</summary>
    uint32_t _spriteIndex = 0;

    /// <summary>How many sprites are in the animastion</summary>
    uint32_t _spriteCount = 0;

    /// <summary>Current time of the animation</summary>
    double _currentTime = 0.0;

    /// <summary>How long one loop of the lasts</summary>
    double _totalTime = 0.0;
};

class AnimationInstance : public SpriteInstance
{
   public:
    AnimationInstance();
    AnimationInstance(AnimatedSprite* animated, glm::vec3 pos = {0, 0, 0}, glm::vec3 scale = {1, 1, 1},
                      glm::vec3 rot = {0, 0, 0});
    ~AnimationInstance();

    /// <summary>Overrides the base render function </summary>
    void render();

   protected:
    AnimatedSprite* _animated;
};
